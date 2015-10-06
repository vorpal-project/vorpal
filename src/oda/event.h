
#ifndef ODA_EVENT_H_
#define ODA_EVENT_H_

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
  using Command = std::tuple<pd::Patch*, std::string, double>;
  Event();
  Status status() const;
  void play();
  void stop();
  bool active() const;
  void pushCommand(const std::string &name, double value = 0.0);
 private:
  Event(pd::Patch *patch);
  static bool popCommand(pd::Patch **patch, std::string *which, double *value);
  static const std::unordered_set<pd::Patch*>& patches();
  static pd::Patch* to_be_closed();
  std::shared_ptr<EventImpl> impl_;
  friend class DSPServer;
};

} // namespace oda

#endif // ODA_EVENT_H_

