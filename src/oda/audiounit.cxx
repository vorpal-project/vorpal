
#include <oda/audiounit.h>

#include <oda/audioserver.h>

namespace oda {

namespace {
using std::vector;
} // unnamed namespace

Status AudioUnit::Null::status() const {
  return Status::INVALID("Null audio unit");
}

void AudioUnit::Null::stream(const vector<float>&) {
  // nothing
}

} // namespace oda

