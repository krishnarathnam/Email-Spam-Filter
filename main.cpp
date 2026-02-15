#include "include/get_token.h"
#include "include/walkDirectory.h"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

std::pair<int, int>
calculate_score(std::unordered_map<std::string, int> &content_freq,
                std::unordered_map<std::string, int> &ham_freq,
                std::unordered_map<std::string, int> &spam_freq, int spam_count,
                int ham_count, int vocab_size, float log_ham_probability,
                float log_spam_probability) {

  double ham_dp = 0.0;
  double spam_dp = 0.0;
  for (auto [word, count] : content_freq) {

    spam_dp += count * std::log((spam_freq[word] + 1.0) /
                                (double)(spam_count + vocab_size));

    ham_dp += count * std::log((ham_freq[word] + 1.0) /
                               (double)(ham_count + vocab_size));
  }

  double spam_score = log_spam_probability + spam_dp;
  double ham_score = log_ham_probability + ham_dp;

  auto p = std::make_pair(ham_score, spam_score);

  return p;
}

int main() {
  std::string path = "./preprocessed/";

  std::unordered_map<std::string, int> ham_freq;
  std::unordered_map<std::string, int> spam_freq;

  auto counts = walkDirectory(path, spam_freq, ham_freq);
  int ham_count = counts.first;
  int spam_count = counts.second;

  int total_count = ham_count + spam_count;

  float ham_probability = float(ham_count) / total_count;
  float spam_probability = float(spam_count) / total_count;

  float log_ham_probability = std::log(ham_probability);
  float log_spam_probability = std::log(spam_probability);

  std::unordered_set<std::string> vocab;

  for (auto &p : ham_freq)
    vocab.insert(p.first);
  for (auto &p : spam_freq)
    vocab.insert(p.first);

  int vocab_size = vocab.size();

  for (auto &entry : std::filesystem::recursive_directory_iterator(
           "./preprocessed/enron6/spam/")) {
    if (entry.is_regular_file()) {
      std::ifstream file(entry.path());
      if (file) {
        std::unordered_map<std::string, int> content_freq;
        int content_count = get_token(file, content_freq);
        auto score = calculate_score(content_freq, ham_freq, spam_freq,
                                     spam_count, ham_count, vocab_size,
                                     log_ham_probability, log_spam_probability);

        double ham_score = score.first;
        double spam_score = score.second;

        std::cout << entry.path().string() << " => "
                  << ((ham_score > spam_score) ? "HAM\n" : "SPAM\n");
      }
    }
  }
}
