
#include <oda/dspunit.h>

#include <string>

namespace oda {

namespace {

using std::string;
using std::vector;

} // unnamed namespace

class DSPUnit::NullImpl : public DSPUnit::Impl {
 public:
  ~NullImpl() {}
  Status status() const override { return Status::INVALID("Null dsp unit"); }
  void processTick() override {}
  void pushCommand(const string &identifier,
                   const vector<Parameter> &parameters) override {}
 private:
  friend class DSPUnit;
  NullImpl() {}
};


DSPUnit::DSPUnit() : impl_(new NullImpl) {}

DSPUnit::DSPUnit(Impl *impl) : impl_(impl) {}

Status DSPUnit::status() const {
  return impl_->status();
}

void DSPUnit::pushCommand(const string &identifier,
                        const vector<Parameter> &parameters) {
  impl_->pushCommand(identifier, parameters);
}

} // namespace oda

