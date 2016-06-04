
#include <oda/dspserver.h>

#include <oda/audiounit.h>
#include <oda/dspunit.h>
#include <oda/engine.h>
#include <oda/parameter.h>

#include <libpd/PdBase.hpp>
#include <libpd/PdReceiver.hpp>
#include <libpd/PdTypes.hpp>

#include <algorithm>
#include <deque>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>

namespace oda {

namespace {

using pd::Patch;
using pd::PdBase;
using pd::PdReceiver;
using std::bind;
using std::deque;
using std::fstream;
using std::make_shared;
using std::mem_fn;
using std::plus;
using std::shared_ptr;
using std::string;
using std::transform;
using std::unique_ptr;
using std::unordered_set;
using std::vector;

class Receiver : public PdReceiver {
 public:
  void print(const string &message) override;
};

using Command = std::tuple<Patch*, string, vector<Parameter>>;

const int             TICK_RATIO = 1;

bool                  started = false;
PdBase                dsp;
unique_ptr<Receiver>  receiver;
float                 inbuf[6400], outbuf[6400];
vector<string>        search_paths;

// Patch management
deque<Command>        commands__;
deque<Patch*>         to_be_closed__;

void Receiver::print(const string &message) {
  std::printf("%s\n", message.c_str());
}

void addNumber(float number) {
  dsp.addFloat(number);
}

void addSymbol(const string &symbol) {
  dsp.addSymbol(symbol);
}

bool checkPath (const string &path) {
  fstream check;
  check.open(path, fstream::in);
  if (check.fail())
    return false;
  check.close();
  return true;
}

} // unnamed namespace

// nested class DSPServer::UnitImpl

class DSPServer::UnitImpl final : public DSPUnit {
 public:
  UnitImpl(Patch *patch);
  ~UnitImpl();
  Status status() const override { return Status::OK("Valid dsp unit"); }
  void transferSignal(shared_ptr<AudioUnit> audio_unit) override;
  void pushCommand(const string &identifier,
                   const vector<Parameter> &parameters) override;
 private:
  friend class DSPServer;
  static bool popCommand(pd::Patch **patch, std::string *identifier,
                         std::vector<Parameter> *parameters);
  static pd::Patch* to_be_closed();
  Patch                           *patch_;
  vector<float>                   buffer_;
  static unordered_set<UnitImpl*> units__;
};

unordered_set<DSPServer::UnitImpl*> DSPServer::UnitImpl::units__;

DSPServer::UnitImpl::UnitImpl(Patch *patch)
  : patch_(patch), buffer_(Engine::TICK_BUFFER_SIZE, 0.0f) {
  units__.insert(this);
}

DSPServer::UnitImpl::~UnitImpl() {
  to_be_closed__.push_back(patch_);
  units__.erase(this);
}

void DSPServer::UnitImpl::transferSignal(shared_ptr<AudioUnit> audio_unit) {
  audio_unit->stream(buffer_);
}

void DSPServer::UnitImpl::pushCommand(const string &identifier,
                                       const vector<Parameter> &parameters) {
  commands__.emplace_back(patch_, identifier, parameters);
}

bool DSPServer::UnitImpl::popCommand(pd::Patch **patch, string *identifier,
                                     vector<Parameter> *parameters) {
  if (commands__.empty())
    return false;
  Command command = commands__.front();
  *patch = std::get<0>(command);
  *identifier = std::get<1>(command);
  *parameters = std::get<2>(command);
  commands__.pop_front();
  return true;
}

Patch* DSPServer::UnitImpl::to_be_closed() {
  if (to_be_closed__.empty())
    return nullptr;
  Patch *patch = to_be_closed__.front();
  to_be_closed__.pop_front();
  return patch;
}

// Enclosing class DSPServer

Status DSPServer::start(const vector<string>& patch_paths) {
  if (started)
    return Status::FAILURE("DSP Server already started");
  if (dsp.init(1, 1, sample_rate())) {
    started = true;
    search_paths.clear();
    for (const string& path : patch_paths)
      addPath(path);
    dsp.computeAudio(true);
    receiver.reset(new Receiver);
    dsp.setReceiver(receiver.get());
    return Status::OK("DSP Server started succesfully");
  }
  return Status::FAILURE("DSP Server could not start");
}

shared_ptr<DSPUnit> DSPServer::loadUnit(const string &path) {
  string filename = path + ".pd";
  for (string search_path : search_paths) {
    if (checkPath(search_path+"/"+filename)) {
      Patch check = dsp.openPatch(filename, search_path);
      if (check.isValid()) {
        Patch *patch = new Patch(check);
        return make_shared<UnitImpl>(patch);
      }
    }
  }
  return make_shared<DSPUnit::Null>();
}

size_t DSPServer::sample_rate() const {
  return 44100;
}

int DSPServer::tick_size() const {
  return PdBase::blockSize()*TICK_RATIO;
}

double DSPServer::time_per_tick() const {
  return 1.0*tick_size()/sample_rate();
}

void DSPServer::addPath(const string &path) {
  dsp.addToSearchPath(path);
  search_paths.push_back(path);
}

void DSPServer::handleCommands() {
  using namespace std::placeholders;
  Patch             *patch;
  string            identifier;
  vector<Parameter> parameters;
  ParameterSwitch   switcher(&addNumber, &addSymbol);
  while (UnitImpl::popCommand(&patch, &identifier, &parameters)) {
    dsp.startMessage();
    for (Parameter param : parameters)
      switcher.handle(param);
    dsp.finishMessage(patch->dollarZeroStr() + "-command", identifier);
  }
}

void DSPServer::process(int ticks, vector<float> *signal) {
  // Process signal
  vector<float> temp;
  signal->resize(ticks*tick_size(), 0.0f);
  for (int i = 0; i < ticks; ++i) {
    // Process global signal
    dsp.processFloat(TICK_RATIO, inbuf, outbuf);
    // Collect processed audio
    for (UnitImpl *unit : UnitImpl::units__) {
      Patch *patch = unit->patch_;
      const string array_name = "openda-bus-"+patch->dollarZeroStr();
      if (dsp.readArray(array_name, temp, tick_size()))
        for (int k = 0; k < tick_size(); ++k)
          (*signal)[k + i*tick_size()] += temp[k];
    }
  }
}

void DSPServer::processTick() {
  dsp.processFloat(TICK_RATIO, inbuf, outbuf);
  for (UnitImpl *unit : UnitImpl::units__) {
    const string array_name = "openda-bus-"+unit->patch_->dollarZeroStr();
    if (!dsp.readArray(array_name, unit->buffer_, tick_size()))
      ; // FIXME houston...
  }
}

void DSPServer::cleanUp() {
  Patch *patch;
  while ((patch = UnitImpl::to_be_closed())) {
    if (patch->isValid()) {
      dsp.closePatch(*patch);
    }
    delete patch;
  }
}

void DSPServer::finish() {
  cleanUp();
}

} // namespace oda

