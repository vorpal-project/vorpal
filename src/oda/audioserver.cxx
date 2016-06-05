
#include <oda/audioserver.h>

#include <oda/audiounit.h>
#include <oda/engine.h>

#include <algorithm>
#include <iostream>

namespace oda {

namespace {

using std::make_shared;
using std::shared_ptr;
using std::transform;
using std::vector;

bool isSourcePlaying(int source) {
  int state;
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

} // unnamed namespace

class AudioServer::UnitImpl final : public AudioUnit {
 public:
  ~UnitImpl() { server_->freeUnit(this); }
  Status status() const override { return Status::OK("Valid audio unit"); }
  void setPosition(float x, float y, float z) override;
  void stream(const vector<float> &signal) override;
 private:
  friend class AudioServer;
  UnitImpl(AudioServer* server, size_t unit_id)
    : server_(server), unit_id_(unit_id) {}
  AudioServer *server_;
  size_t      unit_id_;
};

void AudioServer::UnitImpl::setPosition(float x, float y, float z) {
  server_->setSourcePosition(unit_id_, x, y, z);
}

void AudioServer::UnitImpl::stream(const vector<float> &signal) {
   vector<int16_t> samples(signal.size());
   transform(signal.begin(), signal.end(), samples.begin(),
            [] (float sample) -> int16_t {
              return static_cast<int16_t>(sample*32767.f/2.f);
            });
   server_->streamData(unit_id_, samples);
}

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

shared_ptr<AudioUnit> AudioServer::loadUnit() {
  if (free_sources_.empty())
    return make_shared<AudioUnit::Null>();
  else {
    shared_ptr<AudioUnit> unit(new UnitImpl(this, free_sources_.front()));
    free_sources_.pop();
    return unit;
  }
}

void AudioServer::freeUnit(const UnitImpl *unit) {
  free_sources_.push(unit->unit_id_);
}

// Set Source parameters
void AudioServer::setSourcePosition(size_t source, float x, float y, float z) {
  alSource3f(sources_[source], AL_POSITION, x, y, z);
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

void AudioServer::streamData(size_t source_id, const vector<int16_t> &samples) {
  if (free_buffers_.size() > 0) {
    ALuint buffer = free_buffers_.front();
    ALuint source = sources_[source_id];
    free_buffers_.pop();
    fillBuffer(buffer, samples.data(), samples.size()*sizeof(int16_t));
    alSourceQueueBuffers(source, 1, &buffer);
    if (!isSourcePlaying(source))
      alSourcePlay(source);
  }
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
