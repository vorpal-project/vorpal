#include <oda/player.h>

namespace oda {

  void waitFor(int secs) {
    std::this_thread::sleep_for (std::chrono::seconds(secs));
  }

  uint16_t *generateSineWave(int seconds, float frequency){
    unsigned sample_rate = 44000;
    size_t buf_size = seconds * sample_rate * sizeof(uint16_t);

    uint16_t *samples;
    samples = new uint16_t[buf_size];
    for (int i=0; i<buf_size; i++) {
      // 32760 because we're creating a MONO16 sound, and 16 bits integers goes
      // from -32768 to 32767
      samples[i] = 32760 * sin( (2.f*float(M_PI)*frequency)/sample_rate * i );
    }

    return samples;
  }


  Player::Player() {
    // Setting up Buffers and Sources
    alGenBuffers(NUM_BUFFERS, Buffers);
    alGenSources(NUM_SOURCES, Sources);
  }

  Player::~Player() {
    alDeleteBuffers(NUM_BUFFERS, Buffers);
    alDeleteSources(NUM_SOURCES, Sources);
  }

  void Player::playSource(int sourceNumber) {
    alSourcePlay(Sources[sourceNumber]);
  }

  void Player::playAllSources() {
    alSourcePlayv(NUM_SOURCES, Sources);
  }

  void Player::fillBuffer(ALuint buffer, ALenum format, ALvoid *dataSamples, ALsizei bufferSize, ALsizei sampleRate) {
    alBufferData(buffer, format, dataSamples, bufferSize, sampleRate);
  }

  void Player::setSourcePosition(int source, float X, float Y, float Z) {
    alSource3i(Sources[source], AL_POSITION, X, Y, Z);
  }

  void Player::playSoundOnSource(int seconds, uint16_t *data) {
    fillBuffer(Buffers[0], AL_FORMAT_MONO16, data, sizeof(*data) * 44000 * seconds, 44000);
    alSourcei(Sources[0], AL_BUFFER, Buffers[0]);
    playSource(0);
    waitFor(seconds);
  }

  void Player::playSoundOnSource(ALuint source, ALuint buffer, ALenum format, ALvoid *data, ALsizei bufferSize, ALsizei sampleRate, int seconds) {
    fillBuffer(buffer, format, data, bufferSize, sampleRate);
    alSourcei(source, AL_BUFFER, buffer);
    playSource(source);
    waitFor(seconds);
  }

  void Player::playSineWave (int seconds, float frequency) {
    playSoundOnSource(seconds, generateSineWave(seconds, frequency));
  }


}



//void playScale () {
//  ALuint Buffers[8];
//  alGenBuffers(8, Buffers);
//
//  // Creating the Sine-Wave
//  unsigned sample_rate = 22000;
//  size_t buf_size = 1 * sample_rate * sizeof(uint16_t);
//
//  uint16_t *samples1;
//  uint16_t *samples2;
//  uint16_t *samples3;
//  uint16_t *samples4;
//  uint16_t *samples5;
//  uint16_t *samples6;
//  uint16_t *samples7;
//  uint16_t *samples8;
//  samples1  = new uint16_t[buf_size];
//  samples2  = new uint16_t[buf_size];
//  samples3  = new uint16_t[buf_size];
//  samples4  = new uint16_t[buf_size];
//  samples5  = new uint16_t[buf_size];
//  samples6  = new uint16_t[buf_size];
//  samples7  = new uint16_t[buf_size];
//  samples8  = new uint16_t[buf_size];
//  for (int i=0; i<buf_size; i++) {
//    // 32760 because we're creating a MONO16 sound, and 16 bits integers goes
//    // from -32768 to 32767
//    samples1[i] = 32760 * sin( (2.f*float(M_PI)*440.0000)/sample_rate * i );
//    samples2[i] = 32760 * sin( (2.f*float(M_PI)*493.8833)/sample_rate * i );
//    samples3[i] = 32760 * sin( (2.f*float(M_PI)*523.2510)/sample_rate * i );
//    samples4[i] = 32760 * sin( (2.f*float(M_PI)*587.3295)/sample_rate * i );
//    samples5[i] = 32760 * sin( (2.f*float(M_PI)*659.2551)/sample_rate * i );
//    samples6[i] = 32760 * sin( (2.f*float(M_PI)*698.4564)/sample_rate * i );
//    samples7[i] = 32760 * sin( (2.f*float(M_PI)*783.9908)/sample_rate * i );
//    samples8[i] = 32760 * sin( (2.f*float(M_PI)*880.0000)/sample_rate * i );
//  }
//
//  alBufferData(Buffers[0], AL_FORMAT_MONO16, samples1, buf_size, sample_rate);
//  alBufferData(Buffers[1], AL_FORMAT_MONO16, samples2, buf_size, sample_rate);
//  alBufferData(Buffers[2], AL_FORMAT_MONO16, samples3, buf_size, sample_rate);
//  alBufferData(Buffers[3], AL_FORMAT_MONO16, samples4, buf_size, sample_rate);
//  alBufferData(Buffers[4], AL_FORMAT_MONO16, samples5, buf_size, sample_rate);
//  alBufferData(Buffers[5], AL_FORMAT_MONO16, samples6, buf_size, sample_rate);
//  alBufferData(Buffers[6], AL_FORMAT_MONO16, samples7, buf_size, sample_rate);
//  alBufferData(Buffers[7], AL_FORMAT_MONO16, samples8, buf_size, sample_rate);
//
//  ALuint Source;
//  alGenSources(1, &Source);
//
//  alSourceQueueBuffers(Source, 8, Buffers);
//
//  alSource3i(Source, AL_POSITION, 0, 0, -1);
//  alSourcei(Source, AL_SOURCE_RELATIVE, AL_TRUE);
//  alSourcei(Source, AL_ROLLOFF_FACTOR, 0); 
//
//  alSourcePlay(Source);
//
//  std::this_thread::sleep_for (std::chrono::seconds(8));
//
//  alDeleteBuffers(8, Buffers);
//  alDeleteSources(1, &Source);
//  delete[] samples1;
//  delete[] samples2;
//  delete[] samples3;
//  delete[] samples4;
//  delete[] samples5;
//  delete[] samples6;
//  delete[] samples7;
//  delete[] samples8;
//}