
#ifndef LIBODA_AUDIOUNIT_H_
#define LIBODA_AUDIOUNIT_H_

#include <oda/status.h>

#include <vector>

namespace oda {

class AudioUnit {
 public:
  virtual ~AudioUnit() {}
  virtual Status status() const = 0;
  virtual void stream(const std::vector<float> &signal);
 private:
  friend class AudioServer;
  class Null;
  AudioUnit() {}
};

class AudioUnit::Null final : public AudioUnit {
 public:
  Status status() const override;
  void stream(const std::vector<float>&) override;
};

} // namespace oda

#endif // LIBODA_AUDIOUNIT_H_

