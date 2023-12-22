#include <fstream>
#include <iostream>
#include <vector>
#include <thread>

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
using std::chrono::milliseconds;
using std::thread;
using std::this_thread::sleep_for;

int main() {
  time_t const kWaitInputForSec = 10;
  int const kInputCheckCooldownMs = 100;

  ifstream input("input.txt");
  input >> noskipws;

  vector<streamoff> newline_offsets;
  newline_offsets.push_back(input.tellg());

  for (char symbol; input >> symbol;)
    if (symbol == '\n')
      newline_offsets.push_back(input.tellg());

  input.close();
  input = ifstream("input.txt");

  for (size_t line_number;; line_number = 0) {
    bool got_input = false;
    time_t start_time = time(nullptr);

    thread input_thread([&]() {
      cin >> line_number;
      got_input = true;
    });
    input_thread.detach();

    while (time(nullptr) < start_time + kWaitInputForSec && !got_input)
      sleep_for(milliseconds(kInputCheckCooldownMs));

    if (line_number == 0)
      break;

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