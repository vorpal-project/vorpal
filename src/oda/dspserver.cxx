
#include <oda/dspserver.h>
#include <oda/dspunit.h>
#include <oda/parameter.h>

#include <libpd/PdBase.hpp>
#include <libpd/PdReceiver.hpp>
#include <libpd/PdTypes.hpp>

#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>
#include <memory>

namespace {
using std::bind;
using std::fstream;
using std::mem_fn;
using std::plus;
using std::string;
using std::transform;
using std::unique_ptr;
using std::vector;
using pd::PdBase;
using pd::PdReceiver;
using pd::Patch;
} // unnamed namespace

namespace oda {

// unnamed namespace
namespace {

class Receiver : public PdReceiver {
 public:
  void print(const string &message) override;
};

const int             TICK_RATIO = 1;

bool                  started = false;
PdBase                dsp;
unique_ptr<Receiver>  receiver;
float                 inbuf[6400], outbuf[6400];
vector<string>        search_paths;

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

int DSPServer::sample_rate() const {
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

DSPUnit DSPServer::loadUnit(const string &path) {
  string filename = path + ".pd";
  for (string search_path : search_paths) {
    if (checkPath(search_path+"/"+filename)) {
      Patch check = dsp.openPatch(filename, search_path);
      if (check.isValid()) {
        Patch *patch = new Patch(check);
        return DSPUnit(patch);
      }
    }
  }
  return DSPUnit();
}

void DSPServer::handleCommands() {
  using namespace std::placeholders;
  Patch             *patch;
  string            identifier;
  vector<Parameter> parameters;
  ParameterSwitch   switcher(&addNumber, &addSymbol);
  while (DSPUnit::popCommand(&patch, &identifier, &parameters)) {
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
    for (Patch *patch : DSPUnit::patches()) {
      const string array_name = "openda-bus-"+patch->dollarZeroStr();
      if (dsp.readArray(array_name, temp, tick_size()))
        for (int k = 0; k < tick_size(); ++k)
          (*signal)[k + i*tick_size()] += temp[k];
    }
  }
}

void DSPServer::cleanUp() {
  Patch *patch;
  while (patch = DSPUnit::to_be_closed()) {
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

