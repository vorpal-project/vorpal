
#include <oda/dspunit.h>

#include <oda/dspserver.h>
#include <oda/engine.h>

#include <libpd/PdTypes.hpp>

#include <array>
#include <string>
#include <deque>

namespace oda {

namespace {

using std::array;
using std::deque;
using std::string;
using std::unordered_set;
using std::vector;
using pd::Patch;

deque<DSPUnit::Command> commands__;
unordered_set<Patch*> patches__;
deque<Patch*>         to_be_closed__;

} // unnamed namespace

class DSPUnitImpl {
 public:
  virtual ~DSPUnitImpl() {}
  virtual Status status() const = 0;
  virtual void pushCommand(const string &identifier,
                           const vector<Parameter> &parameters) = 0;
 protected:
  DSPUnitImpl() {}
};

class DSPUnitNullImpl : public DSPUnitImpl {
 public:
  DSPUnitNullImpl() {}
  ~DSPUnitNullImpl() {}
  Status status() const override { return Status::INVALID("Null dsp unit"); }
  void pushCommand(const string &identifier,
                   const vector<Parameter> &parameters) override {}
};

class DSPUnitRealImpl : public DSPUnitImpl {
 public:
  DSPUnitRealImpl(Patch *patch);
  ~DSPUnitRealImpl();
  Status status() const override { return Status::OK("Valid dsp unit"); }
  void pushCommand(const string &identifier,
                   const vector<Parameter> &parameters) override;
 private:
  Patch                                   *patch_;
  array<float, Engine::TICK_BUFFER_SIZE>  buffer_;
};

DSPUnitRealImpl::DSPUnitRealImpl(Patch *patch) : patch_(patch) {
  patches__.insert(patch);
}

DSPUnitRealImpl::~DSPUnitRealImpl() {
  to_be_closed__.push_back(patch_);
  patches__.erase(patch_);
}

void DSPUnitRealImpl::pushCommand(const string &identifier,
                                const vector<Parameter> &parameters) {
  commands__.emplace_back(patch_, identifier, parameters);
}

DSPUnit::DSPUnit() : impl_(new DSPUnitNullImpl) {}

DSPUnit::DSPUnit(Patch *patch) : impl_(new DSPUnitRealImpl(patch)) {}

Status DSPUnit::status() const {
  return impl_->status();
}

void DSPUnit::pushCommand(const string &identifier,
                        const vector<Parameter> &parameters) {
  impl_->pushCommand(identifier, parameters);
}

bool DSPUnit::popCommand(pd::Patch **patch, string *identifier,
                       vector<Parameter> *parameters) {
  if (commands__.empty())
    return false;
  Command command = commands__.front();
  *patch = std::get<0>(command);
  *identifier = std::get<1>(command);
  *parameters = std::get<2>(command);
  commands__.pop_front();
  return true;
}

const unordered_set<Patch*>& DSPUnit::patches() {
  return patches__;
}

Patch* DSPUnit::to_be_closed() {
  if (to_be_closed__.empty())
    return nullptr;
  Patch *patch = to_be_closed__.front();
  to_be_closed__.pop_front();
  return patch;
}

} // namespace oda

