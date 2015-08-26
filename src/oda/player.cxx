
#include <oda/player.h>

#include <iostream>

namespace oda {

namespace {

using std::vector;

void waitFor(int secs) {
  std::this_thread::sleep_for (std::chrono::seconds(secs));
}

uint16_t *generateSineWave(int seconds, float frequency){
  unsigned sample_rate = 44100;
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

} // unnamed namespace

// Constructor
// Default options
Player::Player() : bytes_per_sample_(sizeof(uint16_t)), sample_rate_(44100),
                   format_(AL_FORMAT_MONO16) {
  // Setting up buffers and Sources
  alGenBuffers(NUM_BUFFERS, buffers_);
  alGenSources(NUM_SOURCES, sources_);
  for (unsigned i = 0; i < NUM_BUFFERS; ++i)
    free_buffers_.push(buffers_[i]);
}

// Destructor
Player::~Player() {
  alDeleteBuffers(NUM_BUFFERS, buffers_);
  alDeleteSources(NUM_SOURCES, sources_);
}

// Sample Size setter
void Player::setBytesPerSample(size_t size) {
  bytes_per_sample_ = size;
}

// Sample rate setter
void Player::setSampleRate(unsigned rate) {
  sample_rate_ = rate;
}

// Format Setters
void Player::setFormatToMono8() {
  format_ = AL_FORMAT_MONO8;
}

void Player::setFormatToMono16() {
  format_ = AL_FORMAT_MONO16;
}

void Player::setFormatToStereo8() {
  format_ = AL_FORMAT_STEREO8;
}

void Player::setFormatToStereo16() {
  format_ = AL_FORMAT_STEREO16;
}

// Set Source parameters
void Player::setSourcePosition(int source, float X, float Y, float Z) {
  alSource3i(sources_[source], AL_POSITION, X, Y, Z);
}

// Fill buffers_
void Player::fillBuffer(ALuint buffer, const ALvoid *dataSamples,
                        ALsizei bufferSize) {
  alBufferData(buffer, format_, dataSamples, bufferSize, sample_rate_);
}

void Player::update() {
  int processed;
  alGetSourcei(sources_[0], AL_BUFFERS_PROCESSED, &processed);
  while (processed--) {
    ALuint buffer;
    alSourceUnqueueBuffers(sources_[0], 1, &buffer);
    free_buffers_.push(buffer);
  }
}

bool Player::availableBuffers() const {
  return !free_buffers_.empty();
}

void Player::streamData(const vector<int16_t> *data) {
  ALuint buffer = free_buffers_.front();
  free_buffers_.pop();
  fillBuffer(buffer, data->data(), data->size());
  alSourceQueueBuffers(sources_[0], 1, &buffer);
}

// Play Source
void Player::playSource(int source_number) {
  alSourcePlay(sources_[source_number]);
}

void Player::stopSource(int source_number) {
  alSourceStop(sources_[source_number]);
}

void Player::playAllSources() {
  alSourcePlayv(NUM_SOURCES, sources_);
}

void Player::playSoundOnSource(int seconds, ALvoid *data) {
  fillBuffer(buffers_[0], data, bytes_per_sample_ * sample_rate_ * seconds);
  alSourcei(sources_[0], AL_BUFFER, buffers_[0]);
  playSource(0);
  waitFor(seconds);
}

void Player::playSoundOnSource(ALuint source, ALuint buffer, int seconds,
                               ALvoid *data) {
  fillBuffer(buffer, data, bytes_per_sample_ * sample_rate_ * seconds);
  alSourcei(source, AL_BUFFER, buffer);
  playSource(source);
  waitFor(seconds);
}

// Generic Player functions
void Player::playSineWave (int seconds, float frequency) {
  std::cout << "short size: " << sizeof(short) << std::endl;
  std::cout << "uint16_t size: " << sizeof(uint16_t) << std::endl;
  setBytesPerSample(sizeof(uint16_t));
  uint16_t *data = generateSineWave(seconds, frequency);
  playSoundOnSource(seconds, data);
  delete data;
}

}



//void playScale () {
//  ALuint buffers_[8];
//  alGenbuffers_(8, buffers_);
//
//  // Creating the Sine-Wave
//  unsigned sample_rate_ = 22000;
//  size_t buf_size = 1 * sample_rate_ * sizeof(uint16_t);
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
//    samples1[i] = 32760 * sin( (2.f*float(M_PI)*440.0000)/sample_rate_ * i );
//    samples2[i] = 32760 * sin( (2.f*float(M_PI)*493.8833)/sample_rate_ * i );
//    samples3[i] = 32760 * sin( (2.f*float(M_PI)*523.2510)/sample_rate_ * i );
//    samples4[i] = 32760 * sin( (2.f*float(M_PI)*587.3295)/sample_rate_ * i );
//    samples5[i] = 32760 * sin( (2.f*float(M_PI)*659.2551)/sample_rate_ * i );
//    samples6[i] = 32760 * sin( (2.f*float(M_PI)*698.4564)/sample_rate_ * i );
//    samples7[i] = 32760 * sin( (2.f*float(M_PI)*783.9908)/sample_rate_ * i );
//    samples8[i] = 32760 * sin( (2.f*float(M_PI)*880.0000)/sample_rate_ * i );
//  }
//
//  alBufferData(buffers_[0], AL_FORMAT_MONO16, samples1, buf_size, sample_rate_);
//  alBufferData(buffers_[1], AL_FORMAT_MONO16, samples2, buf_size, sample_rate_);
//  alBufferData(buffers_[2], AL_FORMAT_MONO16, samples3, buf_size, sample_rate_);
//  alBufferData(buffers_[3], AL_FORMAT_MONO16, samples4, buf_size, sample_rate_);
//  alBufferData(buffers_[4], AL_FORMAT_MONO16, samples5, buf_size, sample_rate_);
//  alBufferData(buffers_[5], AL_FORMAT_MONO16, samples6, buf_size, sample_rate_);
//  alBufferData(buffers_[6], AL_FORMAT_MONO16, samples7, buf_size, sample_rate_);
//  alBufferData(buffers_[7], AL_FORMAT_MONO16, samples8, buf_size, sample_rate_);
//
//  ALuint Source;
//  alGenSources(1, &Source);
//
//  alSourceQueuebuffers_(Source, 8, buffers_);
//
//  alSource3i(Source, AL_POSITION, 0, 0, -1);
//  alSourcei(Source, AL_SOURCE_RELATIVE, AL_TRUE);
//  alSourcei(Source, AL_ROLLOFF_FACTOR, 0); 
//
//  alSourcePlay(Source);
//
//  std::this_thread::sleep_for (std::chrono::seconds(8));
//
//  alDeletebuffers_(8, buffers_);
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
