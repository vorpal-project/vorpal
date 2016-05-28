
#include <oda/audioserver.h>
#include <oda/engine.h>

#include <iostream>

namespace oda {

namespace {

using std::vector;

void waitFor(int secs) {
  std::this_thread::sleep_for (std::chrono::seconds(secs));
}

constexpr float make_signal(float frequency, size_t t) {
  // 32760 because we're creating a MONO16 sound, and 16 bits integers goes
  // from -32768 to 32767
  return 32760.f * sin( (2.f*M_PI*frequency)/44100 * t );
}

bool isSourcePlaying(int source) {
  int state;
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

void generateSineWave(vector<int16_t> *samples, float frequency){
  const size_t buf_size = Engine::TICK_BUFFER_SIZE;
  const size_t total_size = NUM_BUFFERS*buf_size;
  samples->resize(total_size);

  for (size_t k=0; k < NUM_BUFFERS; ++k)
    for (int i=0; i<buf_size; ++i) {
      const int t = k*buf_size + i;
      (*samples)[t] = .5 * make_signal(frequency, t);
    }
}

} // unnamed namespace

// Constructor
// Default options
AudioServer::AudioServer() : bytes_per_sample_(sizeof(int16_t)), sample_rate_(44100),
                   format_(AL_FORMAT_MONO16) {
  // Setting up buffers and Sources
  alGenBuffers(NUM_BUFFERS, buffers_);
  alGenSources(NUM_SOURCES, sources_);
  for (unsigned i = 0; i < NUM_BUFFERS; ++i)
    free_buffers_.push(buffers_[i]);
}

// Destructor
AudioServer::~AudioServer() {
  alDeleteBuffers(NUM_BUFFERS, buffers_);
  alDeleteSources(NUM_SOURCES, sources_);
}

// Sample Size setter
void AudioServer::setBytesPerSample(size_t size) {
  bytes_per_sample_ = size;
}

// Sample rate setter
void AudioServer::setSampleRate(unsigned rate) {
  sample_rate_ = rate;
}

// Format Setters
void AudioServer::setFormatToMono8() {
  format_ = AL_FORMAT_MONO8;
}

void AudioServer::setFormatToMono16() {
  format_ = AL_FORMAT_MONO16;
}

void AudioServer::setFormatToStereo8() {
  format_ = AL_FORMAT_STEREO8;
}

void AudioServer::setFormatToStereo16() {
  format_ = AL_FORMAT_STEREO16;
}

// Set Source parameters
void AudioServer::setSourcePosition(int source, float X, float Y, float Z) {
  alSource3i(sources_[source], AL_POSITION, X, Y, Z);
}

// Fill buffers_
void AudioServer::fillBuffer(ALuint buffer, const ALvoid *data_samples,
                        ALsizei buffer_size) {
  alBufferData(buffer, AL_FORMAT_MONO16, data_samples, buffer_size,
               sample_rate_);
}

void AudioServer::update() {
  int processed;
  alGetSourcei(sources_[0], AL_BUFFERS_PROCESSED, &processed);
  if (processed > 0) while (processed--) {
    ALuint buffer;
    alSourceUnqueueBuffers(sources_[0], 1, &buffer);
    free_buffers_.push(buffer);
  }
}

bool AudioServer::availableBuffers() const {
  return !free_buffers_.empty();
}

void AudioServer::streamData(const vector<int16_t> *data) {
  streamData(data, 0u, data->size());
}

void AudioServer::streamData(const vector<int16_t> *data, size_t start, size_t len) {
  ALuint buffer = free_buffers_.front();
  free_buffers_.pop();
  fillBuffer(buffer, data->data()+start, len*sizeof(int16_t));
  alSourceQueueBuffers(sources_[0], 1, &buffer);
  if (!isSourcePlaying(sources_[0]))
   alSourcePlay(sources_[0]); 
}

// Play Source
void AudioServer::playSource(int source_number) {
  alSourcePlay(sources_[source_number]);
}

void AudioServer::stopSource(int source_number) {
  alSourceStop(sources_[source_number]);
}

void AudioServer::playAllSources() {
  alSourcePlayv(NUM_SOURCES, sources_);
}

void AudioServer::playSoundOnSource(const vector<int16_t> *samples) {
  const size_t size = Engine::TICK_BUFFER_SIZE;
  for (int i = 0; i < NUM_BUFFERS; ++i) {
    streamData(samples, i*size, size);
  }
  playSource(0);
}

void AudioServer::playSoundOnSource(ALuint source, ALuint buffer, int seconds,
                               ALvoid *data) {
  fillBuffer(buffer, data, bytes_per_sample_ * sample_rate_ * seconds);
  alSourcei(source, AL_BUFFER, buffer);
  playSource(source);
  waitFor(seconds);
}

// Generic AudioServer functions
void AudioServer::playSineWave (int seconds, float frequency) {
  std::cout << "short size: " << sizeof(short) << std::endl;
  std::cout << "int16_t size: " << sizeof(int16_t) << std::endl;
  vector<int16_t> data;
  setBytesPerSample(sizeof(int16_t));
  generateSineWave(&data, frequency);
  playSoundOnSource(&data);
}

}


