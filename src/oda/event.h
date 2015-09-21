
#ifndef ODA_EVENT_H_
#define ODA_EVENT_H_

#include <memory>

// Forward declarations
namespace pd {
class Patch;
}

namespace oda {

class EventImpl;

class Event {
 public:
  Event();
  void play();
  void stop();
  bool active();
  void setParameter(const std::string &name, double value);
 private:
  Event(pd::Patch *patch);
  std::shared_ptr<EventImpl>  impl_;
  friend class Engine;
};

} // namespace oda

#endif // ODA_EVENT_H_

