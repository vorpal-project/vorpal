
#include <oda/parameter.h>

namespace {
using std::string;
} // unnamed namespace

namespace oda {

Parameter::Parameter(const Parameter &rhs)
    : type_(rhs.type_) {
  switch(type_) {
    case Type::NUMBER:
      number_ = rhs.number_;
      break;
    case Type::SYMBOL:
      symbol_ = rhs.symbol_;
      break;
    default:
      break;
  }
}

Parameter::Parameter(float the_number)
    : type_(Type::NUMBER), number_(the_number) {}

Parameter::Parameter(const string &the_symbol)
    : type_(Type::SYMBOL), symbol_(the_symbol) {}

Parameter& Parameter::operator=(const Parameter &rhs) {
  type_ = rhs.type_;
  switch(type_) {
    case Type::NUMBER:
      number_ = rhs.number_;
      break;
    case Type::SYMBOL:
      symbol_ = rhs.symbol_;
      break;
    default:
      break;
  }
  return *this;
}

ParameterSwitch::ParameterSwitch(const NumblerHandler &the_numbher_handler,
                             const SymbolHandler &the_symbol_handler)
    : number_handler_(the_numbher_handler),
      symbol_handler_(the_symbol_handler) {}

void ParameterSwitch::handle(const Parameter &command) {
  switch(command.type_) {
    case Parameter::Type::NUMBER:
      number_handler_(command.number_);
      break;
    case Parameter::Type::SYMBOL:
      symbol_handler_(command.symbol_);
      break;
    default:
      break;
  }
}

} // namespace oda

