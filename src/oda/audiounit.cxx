
#include <oda/audiounit.h>

namespace oda {

namespace {
using std::shared_ptr;
} // unnamed namespace

class AudioUnitImpl {
 public:
  virtual ~AudioUnitImpl() {}
  virtual Status status() const = 0;
 protected:
  AudioUnitImpl() {}
};

class AudioUnitNullImpl : public AudioUnitImpl {
 public:
  ~AudioUnitNullImpl() {}
  Status status() const override { return Status::INVALID("Null audio unit"); }
 private:
  friend class AudioUnit;
  AudioUnitNullImpl() {}
};

class AudioUnitRealImpl : public AudioUnitImpl {
 public:
  ~AudioUnitRealImpl() {}
  Status status() const override { return Status::OK("Valid audio unit"); }
 private:
  friend class AudioUnit;
  AudioUnitRealImpl(size_t unit_id) : unit_id_(unit_id) {}
  size_t unit_id_;
};

AudioUnit::AudioUnit() : impl_(new AudioUnitNullImpl) {}

AudioUnit::AudioUnit(size_t unit_id) : impl_(new AudioUnitRealImpl(unit_id)) {}

Status AudioUnit::status() const {
  return impl_->status();
}

} // namespace oda

