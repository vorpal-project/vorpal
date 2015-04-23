
#ifndef LIBODA_ODA_SYSTEM_H_
#define LIBODA_ODA_SYSTEM_H_

namespace oda {

/// First Open Dynamic Audio main system class.
class System {
 public:
  System();
  ~System() {}

  int start();
  int finish();
};
 
} // namespace oda

#endif // LIBODA_ODA_SYSTEM_H_

