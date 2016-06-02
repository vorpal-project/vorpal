
#include <oda/audiounit.h>

#include <oda/audioserver.h>

namespace oda {

Status AudioUnit::Null::status() const {
  return Status::INVALID("Null audio unit");
}

} // namespace oda

