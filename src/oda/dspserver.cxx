
#include <oda/dspserver.h>

#include <libpd/PdBase.hpp>
#include <libpd/PdTypes.hpp>

#include <unordered_set>

namespace oda {

using std::string;
using std::unordered_set;
using pd::PdBase;
using pd::Patch;

// unnamed namespace
namespace {

const int             FREQ = 44100;
bool                  started = false;
double                time_accumulated = 0.0;
PdBase                dsp;
unordered_set<Patch*> patches;
short                 inbuf[6400], outbuf[6400];

} // unnamed namespace

Status DSPServer::start() {
  if (started)
    return Status::FAILURE("DSP Server already started");
  if (dsp.init(1, 1, FREQ)) {
    started = true;
    time_accumulated = 0.0;
    return Status::OK("DSP Server started succesfully");
  }
  return Status::FAILURE("DSP Server could not start");
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

void DSPServer::tick(double dt) {
  double time = 1.0*PdBase::blockSize()/FREQ;
  time_accumulated += dt/time;
  int ticks = static_cast<int>(time_accumulated);
  time_accumulated -= ticks;
  for (int i = 0; i < ticks; ++i) {
    dsp.processShort(1, inbuf, outbuf);
    for (Patch *patch : patches) {
      //dsp.readArray(patch->dollarZeroStr() + "-output", output);
    }
  }
}

} // namespace oda

