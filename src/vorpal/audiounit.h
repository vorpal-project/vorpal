
#ifndef LIBODA_AUDIOUNIT_H_
#define LIBODA_AUDIOUNIT_H_

#include <vorpal/status.h>

#include <vector>

namespace vorpal {

class AudioUnit {
 public:
  virtual ~AudioUnit() {}
  virtual Status status() const = 0;
  virtual void setPosition(float x, float y, float z) = 0;
  virtual void stream(const std::vector<float> &signal) = 0;
 private:
  friend class AudioServer;
  class Null;
  AudioUnit() {}
};

class AudioUnit::Null final : public AudioUnit {
 public:
  Status status() const override { return Status::INVALID("Null audio unit"); }
  void setPosition(float, float, float) override {}
  void stream(const std::vector<float>&) override {}
};

} // namespace vorpal

#endif // LIBODA_AUDIOUNIT_H_

