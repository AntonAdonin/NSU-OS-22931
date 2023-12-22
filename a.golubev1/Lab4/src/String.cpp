#include "String.h"

namespace lab4 {
using std::make_unique;
using std::size_t;

class String::Impl {
 public:
  Impl() :
      size_{0},
      capacity_{0} {
  }

  void Read(istream& stream) {
    if (size_ > 0)
      size_ = 0;

    for (char symbol; stream >> symbol;) {
      if (symbol == '\r' || symbol == '\n')
        break;

      if (size_ == capacity_) {
        size_t new_capacity = capacity_ == 0
                              ? kBaseCapacity
                              : capacity_ * 2;
        auto new_body = make_unique<char[]>(new_capacity);

        for (size_t i = 0; i < size_; ++i)
          new_body[i] = body_[i];

        body_ = std::move(new_body);
      }

      body_[size_++] = symbol;
    }
  }

  void Write(ostream& stream) {
    for (size_t i = 0; i < size_; ++i)
      stream << body_[i];
  }

  bool IsTerminatorStr() const {
    for (size_t i = 0; i < size_; ++i) {
      bool is_empty_symbol = body_[i] == ' ' || body_[i] == '\n' || body_[i] == '\r';
      if (!is_empty_symbol) {
        if (body_[i] == kTerminatorSymbol)
          return true;
        return false;
      }
    }
    return true;
  }

 private:
  const size_t kBaseCapacity = 256;
  const char kTerminatorSymbol = '.';

  size_t size_;

  size_t capacity_;

  unique_ptr<char[]> body_;
};

String::String() :
    pimpl_{make_unique<Impl>()} {
}

bool String::IsTerminator() const {
  return pimpl_->IsTerminatorStr();
}

istream& operator>>(istream& input, String& string) {
  string.pimpl_->Read(input);
  return input;
}

ostream& operator<<(ostream& output, String& string) {
  string.pimpl_->Write(output);
  return output;
}

String::String(String&& other) noexcept {
  pimpl_ = std::move(other.pimpl_);
  other.pimpl_ = make_unique<Impl>();
}

String::~String() = default;
}