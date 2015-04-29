
#include <oda/system.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace oda {

ALCdevice   *device = nullptr;
ALCcontext  *context = nullptr;

System::System() {}

Status System::start() {
  if (context && device)
    return Status::INVALID("Already started");
  device = alcOpenDevice(nullptr);
  if (!device)
    return Status::FAILURE("Could not open a device");
  context = alcCreateContext(device, nullptr);
  if (!context || alcMakeContextCurrent(context) == ALC_FALSE) {
    if (context)
      alcDestroyContext(context);
    alcCloseDevice(device);
    return Status::FAILURE("Coult not set a context");
  }
  return Status::OK(alcGetString(device, ALC_DEVICE_SPECIFIER));
}

void System::finish() {
  if (!context) return;
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);
  alcCloseDevice(device);
}

} // namespace oda

