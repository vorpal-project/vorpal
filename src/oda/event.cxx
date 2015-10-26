
#include <oda/event.h>
#include <oda/dspserver.h>

#include <libpd/PdTypes.hpp>

#include <string>
#include <deque>

namespace oda {

using std::deque;
using std::string;
using std::unordered_set;
using std::vector;
using pd::Patch;

namespace {

deque<Event::Command> commands__;
unordered_set<Patch*> patches__;
deque<Patch*>         to_be_closed__;

} // unnamed namespace

class EventImpl {
 public:
  virtual ~EventImpl() {}
  virtual Status status() const = 0;
  virtual void play() = 0;
  virtual void stop() = 0;
  virtual bool active() const = 0;
  virtual void pushCommand(const string &identifier,
                           const vector<Parameter> &parameters) = 0;
 protected:
  EventImpl() {}
};

class EventNullImpl : public EventImpl {
 public:
  EventNullImpl() {}
  ~EventNullImpl() {}
  Status status() const override { return Status::INVALID("Null event"); }
  void play() override {}
  void stop() override {}
  bool active() const override { return false; }
  void pushCommand(const string &identifier,
                   const vector<Parameter> &parameters) override {}
};

class EventRealImpl : public EventImpl {
 public:
  EventRealImpl(Patch *patch);
  ~EventRealImpl();
  Status status() const override { return Status::OK("Valid event"); }
  void play() override;
  void stop() override;
  bool active() const override;
  void pushCommand(const string &identifier,
                   const vector<Parameter> &parameters) override;
 private:
  Patch                   *patch_;
  bool                    active_;
};

EventRealImpl::EventRealImpl(Patch *patch) : patch_(patch) {
  patches__.insert(patch);
}

EventRealImpl::~EventRealImpl() {
  to_be_closed__.push_back(patch_);
  patches__.erase(patch_);
}

void EventRealImpl::play() {
  active_ = true;
}

void EventRealImpl::stop() {
  active_ = false;
}

bool EventRealImpl::active() const {
  return active_;
}

void EventRealImpl::pushCommand(const string &identifier,
                                const vector<Parameter> &parameters) {
  commands__.emplace_back(patch_, identifier, parameters);
}

Event::Event() : impl_(new EventNullImpl) {}

Event::Event(Patch *patch) : impl_(new EventRealImpl(patch)) {}

Status Event::status() const {
  return impl_->status();
}

void Event::play() {
  impl_->play();
}

void Event::stop() {
  impl_->stop();
}

bool Event::active() const {
  return impl_->active();
}

void Event::pushCommand(const string &identifier,
                        const vector<Parameter> &parameters) {
  impl_->pushCommand(identifier, parameters);
}

bool Event::popCommand(pd::Patch **patch, string *identifier,
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

const unordered_set<Patch*>& Event::patches() {
  return patches__;
}

Patch* Event::to_be_closed() {
  if (to_be_closed__.empty())
    return nullptr;
  Patch *patch = to_be_closed__.front();
  to_be_closed__.pop_front();
  return patch;
}

} // namespace oda

