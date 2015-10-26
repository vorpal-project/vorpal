
#ifndef ODA_COMMAND_H_
#define ODA_COMMAND_H_

#include <functional>
#include <string>

namespace oda {

class Command {
 public:
  explicit Command(double the_number);
  explicit Command(const std::string &the_symbol);
 private:
  friend class CommandSwitch;
  enum class Type {
    NUMBER, SYMBOL
  };
  Type type_;
  union {
    double number_;
    std::string symbol_;
  };
};

class CommandSwitch {
 public:
  using NumblerHandler = std::function<void (double)>;
  using SymbolHandler = std::function<void (const std::string&)>;
  CommandSwitch(const NumblerHandler &the_numbher_handler,
                const SymbolHandler &the_symbol_handler);
  void handle(const Command &command);
 private:
  NumblerHandler  number_handler_;
  SymbolHandler   symbol_handler_;
};

} // namespace oda

#endif // ODA_COMMAND_H_

