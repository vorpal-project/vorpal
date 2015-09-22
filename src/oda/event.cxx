
#include <oda/event.h>
#include <oda/dspserver.h>

#include <libpd/PdTypes.hpp>

#include <string>

namespace oda {

using std::string;
using pd::Patch;

class EventImpl {
 public:
  virtual ~EventImpl() {}
  virtual Status status() const = 0;
  virtual void play() = 0;
  virtual void stop() = 0;
  virtual bool active() = 0;
  virtual void trigger(const string &name) = 0;
  virtual void setParameter(const string &name, double value) = 0;
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
  bool active() override { return false; }
  void trigger(const string &name) override {}
  void setParameter(const string &name, double value) override {}
};

class EventRealImpl : public EventImpl {
 public:
  EventRealImpl(Patch *patch) : patch_(patch) {}
  ~EventRealImpl() {
    DSPServer().closePatch(patch_);
    patch_ = nullptr;
  }
  Status status() const override { return Status::OK("Valid event"); }
  void play() override;
  void stop() override;
  bool active() override;
  void trigger(const string &name) override;
  void setParameter(const string &name, double value) override;
 private:
  Patch *patch_;
  bool  active_;
};

void EventRealImpl::play() {
  active_ = true;
}

void EventRealImpl::stop() {
  active_ = false;
}

bool EventRealImpl::active() {
  return active_;
}

void EventRealImpl::trigger(const string &name) {
  // TODO
}

void EventRealImpl::setParameter(const string &name, double value) {
  // TODO
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

bool Event::active() {
  return impl_->active();
}

void Event::setParameter(const string &name, double value) {
  impl_->setParameter(name, value);
}

} // namespace oda

