
#ifndef LIBODA_PLAYER_H_
#define LIBODA_PLAYER_H_

#include <cmath>
#include <thread>
#include <chrono>

#include <AL/al.h>
#include <AL/alc.h>

#define NUM_BUFFERS 8
#define NUM_SOURCES 4

namespace oda {
  class Player {
  	public:
  		Player();
  		~Player();
 			void playSource(int sourceNumber);
 			void playAllSources();
 			void fillBuffer(ALuint buffer, ALenum format, ALvoid *dataSamples, ALsizei bufferSize, ALsizei sampleRate);
 			void setSourcePosition(int source, float X, float Y, float Z);
 			void playSoundOnSource(int seconds, uint16_t *data);
 			void playSoundOnSource(ALuint source, ALuint buffer, ALenum format, ALvoid *data, ALsizei bufferSize, ALsizei sampleRate, int seconds);
 			void playSineWave (int seconds, float frequency);

  	private:
  		ALuint Buffers[NUM_BUFFERS];
    	ALuint Sources[NUM_SOURCES];

  };

} // namespace oda

#endif // LIBODA_PLAYER_H_
