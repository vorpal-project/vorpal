
#ifndef ODA_DSPSERVER_H_
#define ODA_DSPSERVER_H_

#include <oda/status.h>
#include <string>

// Forward declarations
namespace pd {
class Patch;
}

namespace oda {

class DSPServer {
 public:
  Status start();
  pd::Patch *loadPatch(const std::string &path);
  void closePatch(pd::Patch *patch);
};

} // namespace oda

#endif // ODA_DSPSERVER_H_

