#include <iostream>

#include "StringList.h"

using std::cin;
using std::cout;
using std::noskipws;
using lab4::StringList;

int main() {
  cin >> noskipws;

  auto stringList = StringList();

  cin >> stringList;

  cout << stringList;

  return 0;
}
