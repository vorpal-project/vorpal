
#ifndef ODA_DSPSERVER_H_
#define ODA_DSPSERVER_H_

#include <oda/status.h>

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

namespace oda {

// Forwatd declaration
class DSPUnit;

class DSPServer {
 public:
  Status start(const std::vector<std::string>& patch_paths);
  std::shared_ptr<DSPUnit> loadUnit(const std::string &path);
  size_t sample_rate() const;
  int tick_size() const;
  double time_per_tick() const;
  void addPath(const std::string &path);
  void handleCommands();
  void process(int ticks, std::vector<float> *signal);
  void processTick();
  void cleanUp();
  void finish();
 private:
  class UnitImpl;
  static std::unordered_set<UnitImpl*> units__;
};

} // namespace oda

#endif // ODA_DSPSERVER_H_

