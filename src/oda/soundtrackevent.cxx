
#include <oda/soundtrackevent.h>

namespace oda {

namespace {
using std::string;
using std::vector;
} // unnamed namespace

SoundtrackEvent::SoundtrackEvent() : dspunit_() {}

SoundtrackEvent::SoundtrackEvent(const DSPUnit &dspunit) : dspunit_(dspunit) {}

Status SoundtrackEvent::status() const {
  return dspunit_.status();
}

void SoundtrackEvent::pushCommand(const string &identifier,
                                  const vector<Parameter> &parameters) {
  dspunit_.pushCommand(identifier, parameters);
}
                                

} // namespace oda

