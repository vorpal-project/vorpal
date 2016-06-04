
#include <oda/soundtrackevent.h>

namespace oda {

namespace {
using std::shared_ptr;
using std::string;
using std::vector;
} // unnamed namespace

SoundtrackEvent::SoundtrackEvent(const shared_ptr<DSPUnit> &dspunit,
                                 const shared_ptr<AudioUnit> &audiounit)
  : dspunit_(dspunit), audiounit_(audiounit) {}

void SoundtrackEvent::pushCommand(const string &identifier,
                                  const vector<Parameter> &parameters) {
  dspunit_->pushCommand(identifier, parameters);
}

void SoundtrackEvent::processAudio() {
  dspunit_->transferSignal(audiounit_);
}
                                

} // namespace oda

