
#include <oda/engine.h>
#include <oda/player.h>
#include <oda/dspserver.h>
#include <oda/event.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace oda {

// unnamed namespace
namespace {

using std::ofstream;
using std::ostream;
using std::string;
using std::transform;
using std::unique_ptr;
using std::vector;

//#define ODA_LOG

ALCdevice           *device = nullptr;
ALCcontext          *context = nullptr;
unique_ptr<Player>  player;
double              time_accumulated = 0.0;
bool                playing_started = false;
#ifdef ODA_LOG
ofstream            out;
void printSample(ostream &out, float sample) {
  int n = static_cast<int>(sample*40.f)+40;
  for (int i = 0; i < n; ++i)
    out << "#";
  out << std::endl;
}
#endif

}

const size_t        Engine::TICK_BUFFER_SIZE = 16*(64*64);

// MACRO MAGIC:
// http://journal.stuffwithstuff.com/2012/01/24/higher-order-macros-in-c/

Engine::Engine() {}

Status Engine::start() {
  // Do not start if the context was already created
  if (context && device)
    return Status::INVALID("Already started");
  // Open device
  device = alcOpenDevice(nullptr);
  if (!device)
    return Status::FAILURE("Could not open a device");
  // Create and set context
  context = alcCreateContext(device, nullptr);
  if (!context || alcMakeContextCurrent(context) == ALC_FALSE) {
    if (context) {
      alcDestroyContext(context);
      context = nullptr;
    }
    alcCloseDevice(device);
    device = nullptr;
    return Status::FAILURE("Could not set a context");
  }
  // Start DSP server
  Status dsp_start = DSPServer().start();
  if (!dsp_start.ok()) {
    alcDestroyContext(context);
    alcCloseDevice(device);
    context = nullptr;
    device = nullptr;
    return Status::FAILURE("Engine internal: " + dsp_start.description());
  }
  // Create audio player
  player.reset(new Player);
  playing_started = false;
  time_accumulated = 0.0;
#ifdef ODA_LOG
  out.open("out");
#endif
  // Tell which device was opened
  return Status::OK(alcGetString(device, ALC_DEVICE_SPECIFIER));
}

void Engine::finish() {
  // Do not finish if it was not started yet
  if (!context) return;
  // Destroy audio player
  player->stopSource(0);
  player.reset();
  // Unset and destroy context
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);
  context = nullptr;
  // Close device
  alcCloseDevice(device);
  device = nullptr;
}

void Engine::tick(double dt) {
  DSPServer dsp;
  // How many dsp ticks are needed for N seconds
  player->update();
  if (player->availableBuffers()) {
    int ticks = TICK_BUFFER_SIZE/dsp.tick_size();
    // Transfer signal from dsp server to audio server
    vector<float> signal;
    dsp.tick(ticks, &signal);
    vector<int16_t> audio(dsp.tick_size()*ticks);
    //transform(signal.begin(), signal.end(), audio.begin(),
    //          [](float sample) -> int16_t { return sample*32767.f/2.f; });
    for (int i = 0; i < signal.size(); ++i)
      audio[i] = static_cast<int16_t>(signal[i]*32767.f/10.f);
    player->streamData(&audio);
    if (!playing_started) {
      player->playSource(0);
      playing_started = true;
    }
#ifdef ODA_LOG
    out << "Buffer update: " << ticks*dsp.tick_size() << std::endl;
    for (unsigned i = 0; i < audio.size(); ++i)
      printSample(out, audio[i]/32767.f);
#endif
  }
}

Status Engine::eventInstance(const string &path_to_event, Event *event_out) {
  DSPServer dsp;
  auto *patch = dsp.loadPatch(path_to_event + ".pd");
  if (patch) {
    *event_out = Event(patch);
    return Status::OK("Event instance successfully created");
  }
  return Status::FAILURE("Event instance could not be created");
}

void Engine::testAudio() {
  player->playSineWave(4, 440.0f);
}

} // namespace oda

