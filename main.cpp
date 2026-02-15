#include "walkDirectory.cpp"
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
  std::string path = "./enron1";
  std::unordered_map<std::string, int> freq;
  int totalCount = walkDirectory(path, freq);
  std::cout << totalCount << std::endl;

  for (auto [word, count] : freq) {
    std::printf("%s => %d\n", word.c_str(), count);
  }

  return 0;
}

// std::vector<std::pair<std::string, int>> top(freq.begin(), freq.end());
// std::partial_sort(top.begin(), top.begin() + 20, top.end(),
//                   [](auto &a, auto &b) { return a.second > b.second; });
// for (int i = 0; i < 20 && i < top.size(); ++i) {
//   float proba = static_cast<float>(top[i].second) / totalCount;
//   std::printf("%s => %d %.5f\n", top[i].first.c_str(), top[i].second, proba);
// }
