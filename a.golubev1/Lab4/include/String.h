#pragma once

#include <memory>
#include <istream>
#include <ostream>

namespace lab4 {
using std::istream;
using std::ostream;
using std::unique_ptr;

class String {
 public:
  String();

  String(String && other) noexcept ;

  bool IsTerminator() const;

  friend istream& operator>>(istream& input, String& string);

  friend ostream& operator<<(ostream& input, String& string);

  ~String();

 private:
  class Impl;
  unique_ptr<Impl> pimpl_;
};
}