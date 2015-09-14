
#ifndef LIBODA_PLAYER_H_
#define LIBODA_PLAYER_H_

#include <AL/al.h>
#include <AL/alc.h>

#include <chrono>
#include <cmath>
#include <queue>
#include <thread>
#include <vector>

#define NUM_BUFFERS 8
#define NUM_SOURCES 4

namespace oda {

class Player {
 public:
  Player();
  ~Player();
  void playSource(int source_number);
  void stopSource(int source_number);
  void playAllSources();
  void update();
  bool availableBuffers() const;
  void fillBuffer(ALuint buffer, const ALvoid *dataSamples, ALsizei bufferSize);
  void streamData (const std::vector<int16_t> *data);
  void streamData (const std::vector<int16_t> *data, size_t start, size_t len);
  void setSourcePosition(int source, float X, float Y, float Z);
  void playSoundOnSource(const std::vector<int16_t> *samples);
  void playSoundOnSource(ALuint source, ALuint buffer, int seconds,
                         ALvoid *data);
  void playSineWave (int seconds, float frequency);
  void setBytesPerSample(size_t size);
  void setSampleRate(unsigned rate);
  void setFormatToMono8();
  void setFormatToMono16();
  void setFormatToStereo8();
  void setFormatToStereo16();

 private:
  ALuint  buffers_[NUM_BUFFERS];
  ALuint  sources_[NUM_SOURCES];
  std::queue<ALuint> free_buffers_;

  size_t bytes_per_sample_;
  int sample_rate_;
  ALenum format_;
};

} // namespace oda

#endif // LIBODA_PLAYER_H_
