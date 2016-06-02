
#ifndef ODA_DSPUNIT_H_
#define ODA_DSPUNIT_H_

#include <oda/parameter.h>
#include <oda/status.h>

#include <vector>

namespace oda {

class DSPUnitImpl;

class DSPUnit {
 public:
  virtual Status status() const = 0;
  virtual void processTick() = 0;
  virtual void pushCommand(const std::string &identifier,
                           const std::vector<Parameter> &parameters) = 0;
  template <typename... Args> void pushCommand(const std::string &identifier,
                                               Args... args);
 private:
  friend class DSPServer;
  class Null;
  template <typename... Args> struct CommandPusher;
  DSPUnit() {}
};

class DSPUnit::Null final : public DSPUnit {
 public:
  Status status() const override;
  void processTick() override {}
  void pushCommand(const std::string&,
                   const std::vector<Parameter>&) override {}
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

