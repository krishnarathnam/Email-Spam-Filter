#include <cctype>
#include <fstream>
#include <unordered_map>

int get_token(std::ifstream &file, std::unordered_map<std::string, int> &freq) {
  std::string line, word;
  int count = 0;
  while (std::getline(file, line)) {
    for (auto c : line) {
      if (std::isalnum(static_cast<unsigned char>(c))) {
        word += std::toupper(c);
      } else if (!word.empty()) {
        freq[word]++;
        word.clear();
        count++;
      }
    }

    if (!word.empty()) {
      freq[word]++;
      word.clear();
      count++;
    }
  }
  return count;
}
