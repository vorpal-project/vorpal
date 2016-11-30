
#ifndef ODA_DSPUNIT_H_
#define ODA_DSPUNIT_H_

#include <vorpal/parameter.h>
#include <vorpal/status.h>

#include <memory>
#include <vector>

namespace vorpal {

class AudioUnit;

class DSPUnit {
 public:
  virtual Status status() const = 0;
  virtual void transferSignal(std::shared_ptr<AudioUnit> audio_unit) = 0;
  virtual void pushCommand(const std::string &identifier,
                           const std::vector<Parameter> &parameters) = 0;
 private:
  friend class DSPServer;
  class Null;
  DSPUnit() {}
};

class DSPUnit::Null final : public DSPUnit {
 public:
  Status status() const override { return Status::INVALID("Null dps unit"); }
  void transferSignal(std::shared_ptr<AudioUnit>) override {}
  void pushCommand(const std::string&,
                   const std::vector<Parameter>&) override {}
};

} // namespace vorpal

#endif // ODA_DSPUNIT_H_

