
#include <oda/engine.h>
#include <oda/audioserver.h>
#include <oda/dspserver.h>
#include <oda/dspunit.h>
#include <oda/portable.h>
#include <oda/soundtrackevent.h>

#include ODA_OPENAL_DIR(al.h)
#include ODA_OPENAL_DIR(alc.h)

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

namespace oda {

// unnamed namespace
namespace {

using std::make_shared;
using std::ofstream;
using std::ostream;
using std::shared_ptr;
using std::string;
using std::transform;
using std::unique_ptr;
using std::vector;
using std::weak_ptr;

//#define ODA_LOG

ALCdevice                         *device = nullptr;
ALCcontext                        *context = nullptr;
unique_ptr<AudioServer>           audioserver;
vector<weak_ptr<SoundtrackEvent>> events__;
double                            lag__ = 0.0;
long long unsigned                tick_counter__ = 0;
bool                              playing_started = false;

ofstream            out;
void printSample(ostream &out, float sample) {
  int n = static_cast<int>(sample*40.f)+40;
  for (int i = 0; i < n; ++i)
    out << "#";
  out << std::endl;
}

} // unnamed namespace

const size_t Engine::TICK_BUFFER_SIZE = 64;

Engine::Engine() {}

Status Engine::start(const vector<string>& patch_paths) {
  // Do not start if the context was already created
  if (started())
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
  Status dsp_start = DSPServer().start(patch_paths);
  if (!dsp_start.ok()) {
    alcDestroyContext(context);
    alcCloseDevice(device);
    context = nullptr;
    device = nullptr;
    return Status::FAILURE("Engine internal: " + dsp_start.description());
  }
  // Create audio audioserver
  audioserver.reset(new AudioServer);
  playing_started = false;
  lag__ = 0.0;
  tick_counter__ = 0u;
#ifdef ODA_LOG
  out.open("out");
#else
  out.open("/dev/null");
#endif
  // Tell which device was opened
  return Status::OK(alcGetString(device, ALC_DEVICE_SPECIFIER));
}

bool Engine::started() const {
  return context && device;
}

void Engine::registerPath(const string &path) {
  DSPServer().addPath(path);
}

void Engine::finish() {
  // Do not finish if it was not started yet
  if (!context) return;
  // Finish DSP server
  DSPServer().finish();
  // Destroy audio audioserver
  audioserver->stopSource(0);
  audioserver.reset();
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
  const double TICK = 1.0*TICK_BUFFER_SIZE/dsp.sample_rate();
  lag__ += dt;
  // How many dsp ticks are needed for N seconds
  audioserver->update();
  dsp.cleanUp();
  dsp.handleCommands();
  out << "[ODA] update by " << dt << " seconds" << std::endl;
  while (lag__ >= TICK && audioserver->availableBuffers() >= events__.size()) {
    out << "[ODA] tick " << tick_counter__ << "("
        << audioserver->availableBuffers() << " available buffers)"
        << std::endl;
    dsp.processTick();
    shared_ptr<SoundtrackEvent> event;
    size_t idx = 0;
    for (weak_ptr<SoundtrackEvent> weak : events__) {
      if ((event = weak.lock())) {
        out << "[ODA] processing event " << idx << std::endl;
        event->processAudio();
      } else out << "[ODA] dead event " << idx << std::endl;
      ++idx;
    }
    lag__ -= TICK;
    ++tick_counter__;
  }
}

Status Engine::eventInstance(const string &path_to_dspunit,
                             shared_ptr<SoundtrackEvent> *event_out) {
  shared_ptr<DSPUnit> dspunit = DSPServer().loadUnit(path_to_dspunit);
  if (!dspunit->status().ok())
    return Status::FAILURE("Could not load DSP Unit: "
                           + dspunit->status().description());
  shared_ptr<AudioUnit> audiounit = audioserver->loadUnit();
  if (!audiounit->status().ok())
    return Status::FAILURE("Could not load Audio Unit: "
                           + audiounit->status().description());
  *event_out = make_shared<SoundtrackEvent>(dspunit, audiounit);
  events__.emplace_back(*event_out);
  return Status::OK("Soundtrack event successfully created");
}

} // namespace oda
