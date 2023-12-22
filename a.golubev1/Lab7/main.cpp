#include <fstream>
#include <iostream>
#include <vector>
#include <thread>

using std::ifstream;
using std::vector;
using std::size_t;
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

  vector<string> lines;
  lines.emplace_back("");

  for (string line; getline(input, line); )
    lines.push_back(std::move(line));

  input.close();

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

    if (line_number > lines.size()) {
      cout << "Line out of range" << endl;
      continue;
    }

    cout << line_number << ": " << lines[line_number] << "// of size " << lines[line_number].size() << endl;
  }

  return 0;
}