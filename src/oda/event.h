
#ifndef ODA_EVENT_H_
#define ODA_EVENT_H_

#include <oda/status.h>

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
  Status status() const;
  void play();
  void stop();
  bool active();
  void trigger(const std::string &name);
  void setParameter(const std::string &name, double value);
 private:
  Event(pd::Patch *patch);
  std::shared_ptr<EventImpl>  impl_;
  friend class DSPServer;
};

} // namespace oda

#endif // ODA_EVENT_H_

