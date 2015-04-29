
#ifndef LIBODA_ODA_SYSTEM_H_
#define LIBODA_ODA_SYSTEM_H_

#include <oda/status.h>

namespace oda {

/// First Open Dynamic Audio main system class.
class System {
 public:
  System();
  ~System() {}

  Status start();
  void finish();
};
 
} // namespace oda

#endif // LIBODA_ODA_SYSTEM_H_

