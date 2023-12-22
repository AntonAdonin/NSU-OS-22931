#include <fstream>
#include <iostream>
#include <vector>

using std::ifstream;
using std::noskipws;
using std::vector;
using std::size_t;
using std::streamoff;
using std::string;
using std::getline;
using std::cin;
using std::cout;
using std::endl;

int main() {
  ifstream input("input.txt");
  input >> noskipws;

  vector<streamoff> newline_offsets;
  newline_offsets.push_back(input.tellg());

  for (char symbol; input >> symbol;)
    if (symbol == '\n')
      newline_offsets.push_back(input.tellg());

  input.close();
  input = ifstream("input.txt");

  for (size_t line_number; cin >> line_number && line_number > 0;) {
    if (line_number > newline_offsets.size()) {
      cout << "Line out of range" << endl;
      continue;
    }

    string line;
    input.seekg(newline_offsets[line_number - 1]);
    getline(input, line);
    cout << line_number << ": " << line << "// of size " << line.size() << endl;
  }

  input.close();

  return 0;
}