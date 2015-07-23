
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
 protected:
  EventImpl() {}
};

class EventRealImpl : public EventImpl {
 public:
  EventRealImpl(Patch *patch) : patch_(patch) {}
  ~EventRealImpl() {
    DSPServer().closePatch(patch_);
    patch_ = nullptr;
  }
 private:
  Patch *patch_;
};

class EventNullImpl : public EventImpl {
  public:
    EventNullImpl() {}
    ~EventNullImpl() {}
};

Event::Event() : impl_(new EventNullImpl) {}

Event::Event(Patch *patch) : impl_(new EventRealImpl(patch)) {}

void Event::play() {
  // TODO
}

void Event::stop() {
  // TODO
}

void Event::setParameter(const string &name, double value) {
  // TODO
}

} // namespace oda

