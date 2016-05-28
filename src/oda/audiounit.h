
#ifndef LIBODA_AUDIOUNIT_H_
#define LIBODA_AUDIOUNIT_H_

#include <oda/status.h>

#include <memory>

namespace oda {

class AudioUnit {
 public:
  AudioUnit();
  Status status() const;
 private:
  friend class AudioServer;
  class Impl;
  class NullImpl;
  AudioUnit(Impl *impl);
  std::shared_ptr<Impl> impl_;
};

class AudioUnit::Impl {
 public:
  virtual ~Impl() {}
  virtual Status status() const = 0;
 protected:
  Impl() {}
};

} // namespace oda

#endif // LIBODA_AUDIOUNIT_H_

