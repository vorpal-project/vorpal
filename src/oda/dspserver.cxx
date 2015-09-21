
#include <oda/dspserver.h>

#include <libpd/PdBase.hpp>
#include <libpd/PdTypes.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <unordered_set>

namespace oda {

using std::plus;
using std::string;
using std::transform;
using std::unordered_set;
using std::vector;
using pd::PdBase;
using pd::Patch;

// unnamed namespace
namespace {

const int             TICK_RATIO = 64;

bool                  started = false;
PdBase                dsp;
unordered_set<Patch*> patches;
float                 inbuf[6400], outbuf[6400];

} // unnamed namespace

Status DSPServer::start() {
  if (started)
    return Status::FAILURE("DSP Server already started");
  if (dsp.init(1, 1, sample_rate())) {
    started = true;
    dsp.computeAudio(true);
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

Patch *DSPServer::loadPatch(const string &path) {
  Patch *patch = new Patch(dsp.openPatch(path, "."));
  if (patch->isValid()) {
    patches.insert(patch);
    return patch;
  }
  delete patch;
  return nullptr;
}

void DSPServer::closePatch(Patch *patch) {
  if (patch->isValid()) {
    patches.erase(patch);
    dsp.closePatch(*patch);
  }
  delete patch;
}

void DSPServer::tick(int ticks, vector<float> *signal) {
  vector<float> temp;
  signal->resize(ticks*tick_size(), 0.0f);
  for (int i = 0; i < ticks; ++i) {
    // Notify patches
    for (Patch *patch : patches)
      dsp.sendBang(patch->dollarZeroStr() + "-input");
    // Process global signal
    dsp.processFloat(TICK_RATIO, inbuf, outbuf);
    // Collect individual audio
    for (Patch *patch : patches) {
      dsp.readArray(patch->dollarZeroStr() + "-output", temp, tick_size());
      for (int k = 0; k < tick_size(); ++k)
        (*signal)[k + i*tick_size()] += temp[k];
      //transform(signal->begin(), signal->end(),
      //          temp.begin(), signal->begin() + i*tick_size(), plus<float>());
    }
  }
}

} // namespace oda

