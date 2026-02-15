#include "get_token.cpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

int walkDirectory(std::string &path,
                  std::unordered_map<std::string, int> &freq) {
  int totalCount = 0;
  for (auto &entry : std::filesystem::recursive_directory_iterator(path)) {
    if (std::filesystem::is_regular_file(entry)) {
      std::ifstream file(entry.path(), std::ios::binary);
      if (file) {
        totalCount += get_token(file, freq);
      }
    } else if (std::filesystem::is_directory(entry)) {

      std::cout << "Read directory: " << entry.path() << std::endl;
    }
  }

  return totalCount;
}
