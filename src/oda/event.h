
#ifndef ODA_EVENT_H_
#define ODA_EVENT_H_

#include <oda/parameter.h>
#include <oda/status.h>

#include <memory>
#include <tuple>
#include <vector>
#include <unordered_set>

// Forward declarations
namespace pd {
class Patch;
}

namespace oda {

class EventImpl;

class Event {
 public:
  using Command = std::tuple<pd::Patch*, std::vector<Parameter>>;
  Event();
  Status status() const;
  void play();
  void stop();
  bool active() const;
  //template <typename T, typename... Args>
  //void pushCommand(T first_parameter, Args... args);
  void pushCommand(const std::vector<Parameter> &parameters);
 private:
  friend class DSPServer;
  //template <typename T, typename... Args>
  //struct CommandPusher;
  Event(pd::Patch *patch);
  static bool popCommand(pd::Patch **patch, std::vector<Parameter> *parameters);
  static const std::unordered_set<pd::Patch*>& patches();
  static pd::Patch* to_be_closed();
  std::shared_ptr<EventImpl> impl_;
};

//template <typename... Args>
//struct Event::CommandPusher<double, Args...> {
//  static void push(std::vector<Parameter> *parameters, double first_parameter,
//                   Args... args) {
//    
//  }
//};
//
//template <typename T, typename... Args>
//inline void pushCommand(T first_parameter, Args... args) {
//  if (impl_->active()) {
//    std::vector<Parameter> parameters;
//    CommandPusher<T, Args...>::push(&parameters, first_parameter, args...);
//    pushCommand(parameters);
//  }
//}

} // namespace oda

#endif // ODA_EVENT_H_

