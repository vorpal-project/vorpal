
#ifndef LIBODA_SOUNDTRACKEVENT_H_
#define LIBODA_SOUNDTRACKEVENT_H_

#include <oda/audiounit.h>
#include <oda/dspunit.h>
#include <oda/parameter.h>
#include <oda/status.h>

#include <memory>

namespace oda {

class SoundtrackEvent {
 public:
  SoundtrackEvent(const std::shared_ptr<DSPUnit> &dspunit,
                  const std::shared_ptr<AudioUnit> &audiounit);
  void processAudio();
  void setAudioSource(float x, float y, float z);
  void pushCommand(const std::string &identifier,
                   const std::vector<Parameter> &parameters);
  template <typename... Args> void pushCommand(const std::string &identifier,
                                               Args... args);
 private:
  template <typename... Args> struct CommandPusher;
  SoundtrackEvent() {}
  std::shared_ptr<DSPUnit>   dspunit_;
  std::shared_ptr<AudioUnit> audiounit_;
};

template <>
struct SoundtrackEvent::CommandPusher<> {
  static void push(std::vector<Parameter> *parameters) {
    parameters->size();
  }
};

template <typename T, typename... Args>
struct SoundtrackEvent::CommandPusher<T, Args...> {
  static void push(std::vector<Parameter> *parameters, T parameter,
                   Args... args) {
    parameters->emplace_back(parameter);
    CommandPusher<Args...>::push(parameters, args...);
  }
};

template <typename... Args>
inline void SoundtrackEvent::pushCommand(const std::string &identifier,
                                         Args... args) {
  std::vector<Parameter> parameters;
  CommandPusher<Args...>::push(&parameters, args...);
  pushCommand(identifier, parameters);
}

} // namespace oda

#endif // LIBODA_SOUNDTRACKEVENT_H_

