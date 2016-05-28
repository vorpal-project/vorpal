
#include <oda/audiounit.h>

#include <oda/audioserver.h>

namespace oda {

namespace {
using std::shared_ptr;
} // unnamed namespace

class AudioUnit::NullImpl : public AudioUnit::Impl {
 public:
  ~NullImpl() {}
  Status status() const override { return Status::INVALID("Null audio unit"); }
 private:
  friend class AudioUnit;
  NullImpl() {}
};

AudioUnit::AudioUnit() : impl_(new NullImpl) {}

AudioUnit::AudioUnit(Impl *impl) : impl_(impl) {}

Status AudioUnit::status() const {
  return impl_->status();
}

} // namespace oda

