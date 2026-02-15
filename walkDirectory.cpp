#include "include/get_token.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <utility>

std::pair<int, int> walkDirectory(std::string &path,
                                  std::unordered_map<std::string, int> &spam,
                                  std::unordered_map<std::string, int> &ham) {
  ham.reserve(500000);
  spam.reserve(500000);
  int ham_count = 0;
  int spam_count = 0;
  for (auto &entry : std::filesystem::recursive_directory_iterator(path)) {
    if (entry.is_regular_file()) {
      std::ifstream file(entry.path());
      if (!file)
        continue;
      // std::cout << "trained file: " << entry.path() << std::endl;
      if (entry.path().parent_path().filename() == "ham") {
        ham_count += get_token(file, ham);
      } else if (entry.path().parent_path().filename() == "spam") {
        spam_count += get_token(file, spam);
      }
    } else if (entry.is_directory()) {
      // std::cout << "Reading foler: " << entry.path() << std::endl;
    }
  }

  return {ham_count, spam_count};
}
