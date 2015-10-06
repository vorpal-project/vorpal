
#include <oda/dspserver.h>
#include <oda/event.h>

#include <libpd/PdBase.hpp>
#include <libpd/PdReceiver.hpp>
#include <libpd/PdTypes.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>

namespace oda {

using std::plus;
using std::string;
using std::transform;
using std::unique_ptr;
using std::vector;
using pd::PdBase;
using pd::PdReceiver;
using pd::Patch;

// unnamed namespace
namespace {

class Receiver : public PdReceiver {
 public:
  void print(const string &message) override;
};

const int             TICK_RATIO = 64;

bool                  started = false;
PdBase                dsp;
unique_ptr<Receiver>  receiver;
float                 inbuf[6400], outbuf[6400];
vector<string>        search_paths;

void Receiver::print(const string &message) {
  std::printf("%s\n", message.c_str());
}

} // unnamed namespace

Status DSPServer::start() {
  if (started)
    return Status::FAILURE("DSP Server already started");
  if (dsp.init(1, 1, sample_rate())) {
    started = true;
    search_paths.clear();
    dsp.computeAudio(true);
    receiver.reset(new Receiver);
    dsp.setReceiver(receiver.get());
    std::cout << "DSP tick size: " << tick_size() << std::endl;
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

Event DSPServer::loadEvent(const string &path) {
  string filename = path + ".pd";
  for (string search_path : search_paths) {
    Patch check = dsp.openPatch(filename, search_path);
    if (check.isValid()) {
      Patch *patch = new Patch(check);
      return Event(patch);
    }
  }
  return Event();
}

void DSPServer::handleCommands() {
  Patch   *patch;
  string  which;
  double  value;
  while (Event::popCommand(&patch, &which, &value)) {
    dsp.startMessage();
    dsp.addFloat(value);
    dsp.finishMessage(patch->dollarZeroStr() + "-command", which);
  }
}

void DSPServer::process(int ticks, vector<float> *signal) {
  // Process signal
  vector<float> temp;
  signal->resize(ticks*tick_size(), 0.0f);
  for (int i = 0; i < ticks; ++i) {
    // Notify patches
    for (Patch *patch : Event::patches())
      dsp.sendBang(patch->dollarZeroStr() + "-input");
    // Process global signal
    dsp.processFloat(TICK_RATIO, inbuf, outbuf);
    // Collect individual audio
    for (Patch *patch : Event::patches())
      if (dsp.readArray(patch->dollarZeroStr() + "-output", temp, tick_size()))
        for (int k = 0; k < tick_size(); ++k)
          (*signal)[k + i*tick_size()] += temp[k];
  }
}

void DSPServer::cleanUp() {
  Patch *patch;
  while (patch = Event::to_be_closed()) {
    if (patch->isValid()) {
      dsp.closePatch(*patch);
    }
    delete patch;
  }
}

} // namespace oda

