
#include <oda/system.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace oda {

ALCdevice   *device = nullptr;
ALCcontext  *context = nullptr;

System::System() {}

Status System::start() {
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
    if (context)
      alcDestroyContext(context);
    alcCloseDevice(device);
    return Status::FAILURE("Coult not set a context");
  }
  // Tell which device was opened
  return Status::OK(alcGetString(device, ALC_DEVICE_SPECIFIER));
}

void System::finish() {
  // Do not finish if it was not started yet
  if (!context) return;
  // Unset and destroy context
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);
  // Close device
  alcCloseDevice(device);
}

} // namespace oda

