
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
 			void fillBuffer(ALuint buffer, ALvoid *dataSamples, ALsizei bufferSize);
 			void setSourcePosition(int source, float X, float Y, float Z);
 			void playSoundOnSource(int seconds, ALvoid *data);
 			void playSoundOnSource(ALuint source, ALuint buffer, int seconds, ALvoid *data);
 			void playSineWave (int seconds, float frequency);
 			void setBytesPerSample(size_t size);
 			void setSampleRate(unsigned int rate);
 			void setFormatToMono8();
  		void setFormatToMono16();
  		void setFormatToStereo8();
  		void setFormatToStereo16();

  	private:
  		ALuint Buffers[NUM_BUFFERS];
    	ALuint Sources[NUM_SOURCES];

    	size_t bytesPerSample;
    	unsigned int sample_rate;
    	ALenum format;
  };

} // namespace oda

#endif // LIBODA_PLAYER_H_
