
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
  virtual void play() = 0;
  virtual void stop() = 0;
  virtual bool active() = 0;
  virtual void setParameter(const string &name, double value) = 0;
 protected:
  EventImpl() {}
};

class EventNullImpl : public EventImpl {
 public:
  EventNullImpl() {}
  ~EventNullImpl() {}
  void play() override {}
  void stop() override {}
  bool active() override { return false; }
  void setParameter(const string &name, double value) override {}
};

class EventRealImpl : public EventImpl {
 public:
  EventRealImpl(Patch *patch) : patch_(patch) {}
  ~EventRealImpl() {
    DSPServer().closePatch(patch_);
    patch_ = nullptr;
  }
  void play() override;
  void stop() override;
  bool active() override;
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

Event::Event() : impl_(new EventNullImpl) {}

Event::Event(Patch *patch) : impl_(new EventRealImpl(patch)) {}

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

