
#include <oda/engine.h>
#include <oda/player.h>
#include <oda/dspserver.h>

#include <libpd/z_libpd.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <memory>

namespace oda {

// unnamed namespace
namespace {

using std::unique_ptr;

ALCdevice           *device = nullptr;
ALCcontext          *context = nullptr;
unique_ptr<Player>  player;

}

// MACRO MAGIC: http://journal.stuffwithstuff.com/2012/01/24/higher-order-macros-in-c/

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
  // Tell which device was opened
  return Status::OK(alcGetString(device, ALC_DEVICE_SPECIFIER));
}

void Engine::finish() {
  // Do not finish if it was not started yet
  if (!context) return;
  // Destroy audio player
  player.reset();
  // Unset and destroy context
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);
  context = nullptr;
  // Close device
  alcCloseDevice(device);
  device = nullptr;
}

void Engine::testAudio() {
  player->playSineWave(4, 440.0f);
}

} // namespace oda

