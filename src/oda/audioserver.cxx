
#include <oda/audioserver.h>

#include <oda/audiounit.h>
#include <oda/engine.h>

#include <iostream>

namespace oda {

namespace {

using std::vector;

bool isSourcePlaying(int source) {
  int state;
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

} // unnamed namespace

// Constructor
// Default options
AudioServer::AudioServer()
  : sources_(NUM_SOURCES), bytes_per_sample_(sizeof(int16_t)),
    sample_rate_(44100), format_(AL_FORMAT_MONO16) {
  // Setting up buffers and Sources
  // FIXME: check for errors
  alGenBuffers(NUM_BUFFERS, buffers_);
  for (unsigned i = 0; i < NUM_BUFFERS; ++i)
    free_buffers_.push(buffers_[i]);
  alGenSources(NUM_SOURCES, sources_.data());
  for (size_t i = 0; i < sources_.size(); ++i)
    free_sources_.push(i);
}

// Destructor
AudioServer::~AudioServer() {
  alDeleteBuffers(NUM_BUFFERS, buffers_);
  alDeleteSources(NUM_SOURCES, sources_.data());
  sources_.clear();
}

AudioUnit AudioServer::loadUnit() {
  return AudioUnit();
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
  alSourcePlayv(NUM_SOURCES, sources_.data());
}

void AudioServer::playSoundOnSource(const vector<int16_t> *samples) {
  const size_t size = Engine::TICK_BUFFER_SIZE;
  for (int i = 0; i < NUM_BUFFERS; ++i) {
    streamData(samples, i*size, size);
  }
  playSource(0);
}

}


