
#ifndef ODA_DSPSERVER_H_
#define ODA_DSPSERVER_H_

#include <oda/status.h>
#include <string>
#include <vector>

// Forward declarations
namespace pd {
class Patch;
}

namespace oda {

class DSPServer {
 public:
  Status start();
  int sample_rate() const;
  double time_per_tick() const;
  pd::Patch *loadPatch(const std::string &path);
  void closePatch(pd::Patch *patch);
  void tick(std::vector<float> *signal);
};

} // namespace oda

#endif // ODA_DSPSERVER_H_

