#pragma once

#include "String.h"

namespace lab4 {
class StringList {
 public:
  StringList();

  friend istream& operator>>(istream& input, StringList& string);

  friend ostream& operator<<(ostream& output, StringList& string);

  ~StringList();

 private:
  class Impl;
  unique_ptr<Impl> pimpl_;
};
}