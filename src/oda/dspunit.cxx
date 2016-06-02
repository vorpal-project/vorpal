
#include <oda/dspunit.h>

#include <string>

namespace oda {

Status DSPUnit::Null::status() const {
  return Status::INVALID("Null dsp unit");
}

} // namespace oda

