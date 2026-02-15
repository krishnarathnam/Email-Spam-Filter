#include "include/get_token.h"
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <utility>

std::pair<int, int> walkDirectory(std::string &path,
                                  std::unordered_map<std::string, int> &spam,
                                  std::unordered_map<std::string, int> &ham) {
  int ham_count = 0;
  int spam_count = 0;
  int max_files = 15000;
  int file_seen = 0;
  bool is_ham = false;
  for (auto &entry : std::filesystem::recursive_directory_iterator(path)) {
    if (file_seen > max_files)
      break;
    if (entry.is_regular_file()) {
      std::filesystem::path p(entry.path().string());
      std::ifstream file(entry.path());
      if (file) {
        file_seen++;
        std::cout << "trained file: " << entry.path().string() << std::endl;
        if (p.parent_path().filename().string() == "ham") {
          ham_count += get_token(file, ham);
        } else if (p.parent_path().filename().string() == "spam") {
          spam_count += get_token(file, spam);
        }
      }
    } else if (entry.is_directory()) {
      std::cout << "Reading foler: " << entry.path().string() << std::endl;
    }
  }

  auto counts = std::make_pair(ham_count, spam_count);

  return counts;
}
