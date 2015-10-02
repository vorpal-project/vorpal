
#include <oda/event.h>
#include <oda/dspserver.h>

#include <libpd/PdTypes.hpp>

#include <string>
#include <deque>

namespace oda {

using std::string;
using std::deque;
using pd::Patch;

namespace {

deque<Event::Command> commands__;

} // unnamed namespace

class EventImpl {
 public:
  virtual ~EventImpl() {}
  virtual Status status() const = 0;
  virtual void play() = 0;
  virtual void stop() = 0;
  virtual bool active() const = 0;
  virtual void pushCommand(const string &name, double value) = 0;
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
  void pushCommand(const string &name, double value) override {}
};

class EventRealImpl : public EventImpl {
 public:
  EventRealImpl(Patch *patch) : patch_(patch) {}
  ~EventRealImpl() {
    DSPServer().closePatch(patch_);
  }
  Status status() const override { return Status::OK("Valid event"); }
  void play() override;
  void stop() override;
  bool active() const override;
  void pushCommand(const string &name, double value) override;
 private:
  Patch                   *patch_;
  bool                    active_;
};

void EventRealImpl::play() {
  active_ = true;
}

void EventRealImpl::stop() {
  active_ = false;
}

bool EventRealImpl::active() const {
  return active_;
}

void EventRealImpl::pushCommand(const string &name, double value) {
  commands__.emplace_back(patch_, name, value);
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

void Event::pushCommand(const string &name, double value) {
  impl_->pushCommand(name, value);
}

bool Event::popCommand(Command *command_ptr) {
  if (commands__.empty())
    return false;
  *command_ptr = commands__.front();
  return true;
}

} // namespace oda

