
#ifndef LIBODA_AUDIOUNIT_H_
#define LIBODA_AUDIOUNIT_H_

#include <oda/status.h>

#include <memory>

namespace oda {

class AudioUnitImpl;

class AudioUnit {
 public:
  AudioUnit();
  Status status() const;
 private:
  friend class AudioServer;
  AudioUnit(size_t unit_id);
  std::shared_ptr<AudioUnitImpl> impl_;
};

} // namespace oda

#endif // LIBODA_AUDIOUNIT_H_

