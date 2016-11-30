
#ifndef ODA_PARAMETER_H_
#define ODA_PARAMETER_H_

#include <functional>
#include <string>

namespace vorpal {

class Parameter {
 public:
  Parameter (const Parameter &rhs);
  ~Parameter() {}
  Parameter& operator=(const Parameter &rhs);
  explicit Parameter(float the_number);
  explicit Parameter(const std::string &the_symbol);
 private:
  friend class ParameterSwitch;
  enum class Type {
    NUMBER, SYMBOL
  };
  Type          type_;
  union {
    float       number_;
    std::string symbol_;
  };
};

class ParameterSwitch {
 public:
  using NumblerHandler = std::function<void (float)>;
  using SymbolHandler = std::function<void (const std::string&)>;
  ParameterSwitch(const NumblerHandler &the_numbher_handler,
                  const SymbolHandler &the_symbol_handler);
  void handle(const Parameter &parameter);
 private:
  NumblerHandler  number_handler_;
  SymbolHandler   symbol_handler_;
};

} // namespace vorpal

#endif // ODA_PARAMETER_H_

