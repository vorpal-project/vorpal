
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
    return Status::OK("DSP Server started succesfully");
  }
  return Status::FAILURE("DSP Server could not start");
}

int DSPServer::sample_rate() const {
  return 44100;
}

int DSPServer::tick_size() const {
  return PdBase::blockSize();
}

double DSPServer::time_per_tick() const {
  return 1.0*PdBase::blockSize()/sample_rate();
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

//static bool nope = false;

void DSPServer::tick(vector<float> *signal) {
  vector<float> temp;
  dsp.processFloat(1, inbuf, outbuf);
  signal->resize(PdBase::blockSize(), 0.0f);
  for (Patch *patch : patches) {
    dsp.readArray(patch->dollarZeroStr() + "-output", temp);
    transform(signal->begin(), signal->end(),
              temp.begin(), signal->begin(), plus<float>());
  }
  //if (!nope) {
  //  for (unsigned i = 0; i < 8; ++i)
  //    std::cout << (*signal)[i*signal->size()/8] << std::endl;
  //  nope = true;
  //}
}

} // namespace oda

