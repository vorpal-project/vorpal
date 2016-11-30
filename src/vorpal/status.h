
#ifndef LIBODA_ODA_STATUS_H_
#define LIBODA_ODA_STATUS_H_

#include <string>

namespace vorpal {

class Status {
 public:
  static Status OK(const std::string& the_description);
  static Status FAILURE(const std::string& the_description);
  static Status INVALID(const std::string& the_description);
  bool ok() const;
  std::string description() const;
 private:
  enum class Type {
    OK, FAILURE, INVALID
  };
  Status(Type the_type, const std::string& the_description);
  Type        type_;
  std::string description_;
};

inline Status::Status(Type the_type, const std::string& the_description)
    : type_(the_type), description_(the_description) {}

inline Status Status::OK(const std::string& the_description) {
  return Status(Type::OK, the_description);
}

inline Status Status::FAILURE(const std::string& the_description) {
  return Status(Type::FAILURE, the_description);
}

inline Status Status::INVALID(const std::string& the_description) {
  return Status(Type::INVALID, the_description);
}

inline bool Status::ok() const {
  return type_ == Type::OK;
}

inline std::string Status::description() const {
  return description_;
}

} // namespace vorpal

#endif // LIBODA_ODA_STATUS_H_

