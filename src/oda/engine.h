
#ifndef LIBODA_ODA_ENGINE_H_
#define LIBODA_ODA_ENGINE_H_

#include <oda/status.h>

#include <memory>
#include <string>
#include <vector>

namespace oda {

// Forward declarations
class SoundtrackEvent;

/// Open Dynamic Audio engine class.
/** 
 * All instances of this class handle the same shared resources. Thus, there
 * is no need to dynamically allocate objects for it.
 * 
 * Use the `start()` method to initialize the engine, and the `finish()` method
 * to release it. Example:
 *
 * ~~~.cxx
 * oda::Engine engine;
 * oda::Status status = engine.start();
 * // check return value
 * // use the engine
 * engine.finish();
 * ~~~
 * @see oda::Engine::start()
 * @see oda::Engine::finish()
 */
class Engine {
 public:
  Engine();
  ~Engine() {}

  /// Starts the main ODA engine
  /**
   * @return A Status object that tells how the initialization went.
   * @see oda::Status
   * @see oda::Engine::finish()
   */
  Status start(const std::vector<std::string>& patch_paths = {});

  /// Tells whether the engine has already started
  /**
   * @return True if the engine has already started, false otherwise
   * @see oda::start()
   */
  bool started() const;

  /// Addes a path to look for event files
  /**
   *  @param string A path
   */
  void registerPath(const std::string &path);

  /// Finishes the main ODA engine
  /**
   * @see oda::Engine::start()
   */
  void finish();

  /// Process dynamic audio ticks
  /**
   */
  void tick(double dt);

  /// Creates an Event instance
  /**
   * @param path_to_event File path to event data
   * @param event_out Pointer to event output variable
   * @return Status Whether the event was successfully created or not
   */
  Status eventInstance(const std::string &path_to_event,
                       std::shared_ptr<SoundtrackEvent> *event_out);

  const static size_t TICK_BUFFER_SIZE = 64;
};
 
} // namespace oda

#endif // LIBODA_ODA_ENGINE_H_

