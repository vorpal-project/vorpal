
#include <oda/command.h>

namespace {
using std::string;
} // unnamed namespace

namespace oda {

Command::Command(double the_number)
    : type_(Type::NUMBER), number_(the_number) {}

Command::Command(const string &the_symbol)
    : type_(Type::SYMBOL), symbol_(the_symbol) {}

CommandSwitch::CommandSwitch(const NumblerHandler &the_numbher_handler,
                             const SymbolHandler &the_symbol_handler)
    : number_handler_(the_numbher_handler),
      symbol_handler_(the_symbol_handler) {}

void CommandSwitch::handle(const Command &command) {
  switch(command.type_) {
    case Command::Type::NUMBER:
      number_handler_(command.number_);
      break;
    case Command::Type::SYMBOL:
      symbol_handler_(command.symbol_);
      break;
    default:
      break;
  }
}

} // namespace oda

