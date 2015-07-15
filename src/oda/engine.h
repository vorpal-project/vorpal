
#ifndef LIBODA_ODA_ENGINE_H_
#define LIBODA_ODA_ENGINE_H_

#include <oda/status.h>

namespace oda {

/// Open Dynamic Audio engine class.
/** 
 * All instances of this class handle the same shared resources. Thus, there
 * is no need to dynamically allocate objects for it.
 * 
 * Use the `start()` method to initialize the system, and the `finish()` method
 * to release it. Example:
 *
 * ~~~.cxx
 * oda::Engine engine;
 * oda::Status status = engine.start();
 * // check return value
 * // use the system
 * engine.finish();
 * ~~~
 * @see oda::Engine::start()
 * @see oda::Engine::finish()
 */
class Engine {
 public:
  Engine();
  ~Engine() {}

  /// Starts the main ODA system
  /**
   * @return A Status object that tells how the initialization went.
   * @see oda::Status
   * @see oda::Engine::finish()
   */
  Status start();

  /// Finishes the main ODA system
  /**
   * @see oda::Engine::start()
   */
  void finish();
};
 
} // namespace oda

#endif // LIBODA_ODA_ENGINE_H_

