
#include <oda/oda.h>

#include <cstdio>
#include <cmath>
#include <thread>
#include <chrono>


#include <AL/al.h>
#include <AL/alc.h>

namespace oda {

void dummy () {
  printf("It works!\n");
}

void playSineWave (int seconds, float frequency) {
  // Setting up Buffers
  ALuint Buffer;
  alGenBuffers(1, &Buffer);

  // Creating the Sine-Wave
  unsigned sample_rate = 22000;
  size_t buf_size = seconds * sample_rate * sizeof(uint16_t);

  uint16_t *samples;
  samples = new uint16_t[buf_size];
  for (int i=0; i<buf_size; i++) {
    samples[i] = 32760 * sin( (2.f*float(M_PI)*frequency)/sample_rate * i ); // 32760 because we're creating a MONO16 sound, and 16 bits integers goes from -32768 to 32767
  }

  // Fills the buffer with data 
  alBufferData(Buffer, AL_FORMAT_MONO16, samples, buf_size, sample_rate);

  // Setting up source
  ALuint Source;
  alGenSources(1, &Source);
  alSourcei(Source, AL_BUFFER, Buffer);

  
  alSource3i(Source, AL_POSITION, 0, 0, -1);
  alSourcei(Source, AL_SOURCE_RELATIVE, AL_TRUE);
  alSourcei(Source, AL_ROLLOFF_FACTOR, 0); 

  // Play the buffer
  alSourcePlay(Source);
  
  std::this_thread::sleep_for (std::chrono::seconds(seconds));

  alDeleteBuffers(1, &Buffer);
  alDeleteSources(1, &Source);

}

void playScale () {
  ALuint Buffers[8];
  alGenBuffers(8, Buffers);

  // Creating the Sine-Wave
  unsigned sample_rate = 22000;
  size_t buf_size = 1 * sample_rate * sizeof(uint16_t);

  uint16_t *samples1;
  uint16_t *samples2;
  uint16_t *samples3;
  uint16_t *samples4;
  uint16_t *samples5;
  uint16_t *samples6;
  uint16_t *samples7;
  uint16_t *samples8;
  samples1  = new uint16_t[buf_size];
  samples2  = new uint16_t[buf_size];
  samples3  = new uint16_t[buf_size];
  samples4  = new uint16_t[buf_size];
  samples5  = new uint16_t[buf_size];
  samples6  = new uint16_t[buf_size];
  samples7  = new uint16_t[buf_size];
  samples8  = new uint16_t[buf_size];
  for (int i=0; i<buf_size; i++) {
    samples1[i] = 32760 * sin( (2.f*float(M_PI)*440.0000)/sample_rate * i ); // 32760 because we're creating a MONO16 sound, and 16 bits integers goes from -32768 to 32767
    samples2[i] = 32760 * sin( (2.f*float(M_PI)*493.8833)/sample_rate * i );
    samples3[i] = 32760 * sin( (2.f*float(M_PI)*523.2510)/sample_rate * i );
    samples4[i] = 32760 * sin( (2.f*float(M_PI)*587.3295)/sample_rate * i );
    samples5[i] = 32760 * sin( (2.f*float(M_PI)*659.2551)/sample_rate * i );
    samples6[i] = 32760 * sin( (2.f*float(M_PI)*698.4564)/sample_rate * i );
    samples7[i] = 32760 * sin( (2.f*float(M_PI)*783.9908)/sample_rate * i );
    samples8[i] = 32760 * sin( (2.f*float(M_PI)*880.0000)/sample_rate * i );
  }

  alBufferData(Buffers[0], AL_FORMAT_MONO16, samples1, buf_size, sample_rate);
  alBufferData(Buffers[1], AL_FORMAT_MONO16, samples2, buf_size, sample_rate);
  alBufferData(Buffers[2], AL_FORMAT_MONO16, samples3, buf_size, sample_rate);
  alBufferData(Buffers[3], AL_FORMAT_MONO16, samples4, buf_size, sample_rate);
  alBufferData(Buffers[4], AL_FORMAT_MONO16, samples5, buf_size, sample_rate);
  alBufferData(Buffers[5], AL_FORMAT_MONO16, samples6, buf_size, sample_rate);
  alBufferData(Buffers[6], AL_FORMAT_MONO16, samples7, buf_size, sample_rate);
  alBufferData(Buffers[7], AL_FORMAT_MONO16, samples8, buf_size, sample_rate);

  ALuint Source;
  alGenSources(1, &Source);

  alSourceQueueBuffers(Source, 8, Buffers);

  alSource3i(Source, AL_POSITION, 0, 0, -1);
  alSourcei(Source, AL_SOURCE_RELATIVE, AL_TRUE);
  alSourcei(Source, AL_ROLLOFF_FACTOR, 0); 

  alSourcePlay(Source);

  std::this_thread::sleep_for (std::chrono::seconds(8));

  alDeleteBuffers(8, Buffers);
  alDeleteSources(1, &Source);
}

} // namespace oda

