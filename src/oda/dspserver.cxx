
#include <oda/dspserver.h>

#include <libpd/PdBase.hpp>
#include <libpd/PdTypes.hpp>

namespace oda {

using std::string;
using pd::PdBase;
using pd::Patch;

// unnamed namespace
namespace {

bool    started = false;
PdBase  dsp;

} // unnamed namespace

Status DSPServer::start() {
  if (started)
    return Status::FAILURE("DSP Server already started");
  if (dsp.init(1, 1, 44100)) {
    started = true;
    return Status::OK("DSP Server started succesfully");
  }
  return Status::FAILURE("DSP Server could not start");
}

Patch *DSPServer::loadPatch(const string &path) {
  Patch *patch = new Patch(dsp.openPatch(path, "."));
  if (patch->isValid())
    return patch;
  delete patch;
  return nullptr;
}

void DSPServer::closePatch(Patch *patch) {
  if (patch->isValid())
    dsp.closePatch(*patch);
  delete patch;
}

} // namespace oda

