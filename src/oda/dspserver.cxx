
#include <oda/dspserver.h>

#include <libpd/PdBase.hpp>

namespace oda {

// unnamed namespace
namespace {

bool        started = false;
pd::PdBase  dsp;

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

} // namespace oda

