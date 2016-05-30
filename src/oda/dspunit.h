
#ifndef ODA_DSPUNIT_H_
#define ODA_DSPUNIT_H_

#include <oda/parameter.h>
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

class DSPUnitImpl;

class DSPUnit {
 public:
  using Command = std::tuple<pd::Patch*, std::string, std::vector<Parameter>>;
  DSPUnit();
  Status status() const;
  void processTick();
  void pushCommand(const std::string &identifier,
                   const std::vector<Parameter> &parameters);
  template <typename... Args> void pushCommand(const std::string &identifier,
                                               Args... args);
 private:
  friend class DSPServer;
  class Impl;
  class NullImpl;
  template <typename... Args> struct CommandPusher;
  DSPUnit(Impl *impl);
  std::shared_ptr<Impl> impl_;
};

class DSPUnit::Impl {
 public:
  virtual ~Impl() {}
  virtual Status status() const = 0;
  virtual void processTick() = 0;
  virtual void pushCommand(const std::string &identifier,
                           const std::vector<Parameter> &parameters) = 0;
 protected:
  Impl() {}
};

template <>
struct DSPUnit::CommandPusher<> {
  static void push(std::vector<Parameter> *parameters) {
    parameters->size();
  }
};

template <typename T, typename... Args>
struct DSPUnit::CommandPusher<T, Args...> {
  static void push(std::vector<Parameter> *parameters, T parameter,
                   Args... args) {
    parameters->emplace_back(parameter);
    CommandPusher<Args...>::push(parameters, args...);
  }
};

template <typename... Args>
inline void DSPUnit::pushCommand(const std::string &identifier, Args... args) {
  std::vector<Parameter> parameters;
  CommandPusher<Args...>::push(&parameters, args...);
  pushCommand(identifier, parameters);
}

} // namespace oda

#endif // ODA_DSPUNIT_H_

