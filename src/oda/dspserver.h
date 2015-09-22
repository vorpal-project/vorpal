
#ifndef ODA_DSPSERVER_H_
#define ODA_DSPSERVER_H_

#include <oda/status.h>
#include <string>
#include <vector>

// Forward declaration
namespace pd {
class Patch;
}

namespace oda {

// Forwatd declaration
class Event;

class DSPServer {
 public:
  Status start();
  int sample_rate() const;
  int tick_size() const;
  double time_per_tick() const;
  void addPath(const std::string &path);
  Event loadEvent(const std::string &path);
  void closePatch(pd::Patch *patch);
  void tick(int ticks, std::vector<float> *signal);
};

} // namespace oda

#endif // ODA_DSPSERVER_H_

