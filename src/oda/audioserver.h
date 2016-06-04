
#ifndef LIBODA_AUDIOSERVER_H_
#define LIBODA_AUDIOSERVER_H_

#include <oda/portable.h>
#include <oda/status.h>

#include ODA_OPENAL_DIR(al.h)
#include ODA_OPENAL_DIR(alc.h)

#include <chrono>
#include <cmath>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

#define NUM_BUFFERS 64
#define NUM_SOURCES 4

namespace oda {

class AudioUnit;

class AudioServer final {
 public:
  AudioServer();
  ~AudioServer();
  std::shared_ptr<AudioUnit> loadUnit();

  void playSource(int source_number);
  void stopSource(int source_number);
  void playAllSources();
  void update();
  bool availableBuffers() const;
  void fillBuffer(ALuint buffer, const ALvoid *dataSamples, ALsizei bufferSize);
  void streamData (const std::vector<int16_t> *data);
  void streamData (const std::vector<int16_t> *data, size_t start, size_t len);
  void streamData (size_t source_id, const std::vector<int16_t> &samples);
  void setSourcePosition(int source, float X, float Y, float Z);
  void playSoundOnSource(const std::vector<int16_t> *samples);

 protected:
  class UnitImpl;
  friend class UnitImpl;
  void freeUnit(const UnitImpl *unit);

 private:
  ALuint  buffers_[NUM_BUFFERS];
  std::queue<ALuint>  free_buffers_;
  std::vector<ALuint> sources_;
  std::queue<size_t>  free_sources_;

  const size_t bytes_per_sample_;
  const int sample_rate_;
  const ALenum format_;
};

} // namespace oda

#endif // LIBODA_AUDIOSERVER_H_
