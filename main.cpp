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

std::string training_root = "./train/";
std::string test_path = "./test/enron6/ham/";

std::pair<int, int>
calculate_score(std::unordered_map<std::string, int> &doc_freq,
                std::unordered_map<std::string, int> &ham_freq,
                std::unordered_map<std::string, int> &spam_freq, int spam_total,
                int ham_total, int vocab_size, float log_ham_prior,
                float log_spam_prior) {
  double ham_log_sum = 0.0;
  double spam_log_sum = 0.0;

  for (auto [word, count] : doc_freq) {
    spam_log_sum += count * std::log((spam_freq[word] + 1.0) /
                                     (double)(spam_total + vocab_size));

    ham_log_sum += count * std::log((ham_freq[word] + 1.0) /
                                    (double)(ham_total + vocab_size));
  }

  double ham_score = log_ham_prior + ham_log_sum;
  double spam_score = log_spam_prior + spam_log_sum;

  return {ham_score, spam_score};
}

int main() {

  std::unordered_map<std::string, int> ham_freq;
  std::unordered_map<std::string, int> spam_freq;

  auto counts = walkDirectory(training_root, spam_freq, ham_freq);

  int ham_total = counts.first;
  int spam_total = counts.second;
  int total_docs = ham_total + spam_total;

  float ham_prior = float(ham_total) / total_docs;
  float spam_prior = float(spam_total) / total_docs;

  float log_ham_prior = std::log(ham_prior);
  float log_spam_prior = std::log(spam_prior);

  std::unordered_set<std::string> vocab;

  for (auto &p : ham_freq)
    vocab.insert(p.first);
  for (auto &p : spam_freq)
    vocab.insert(p.first);

  int vocab_size = vocab.size();

  int ham_count = 0;
  int spam_count = 0;
  for (auto &entry : std::filesystem::recursive_directory_iterator(test_path)) {
    if (!entry.is_regular_file())
      continue;

    std::ifstream file(entry.path());
    if (!file)
      continue;

    std::unordered_map<std::string, int> doc_freq;
    get_token(file, doc_freq);

    auto [ham_score, spam_score] =
        calculate_score(doc_freq, ham_freq, spam_freq, spam_total, ham_total,
                        vocab_size, log_ham_prior, log_spam_prior);

    // std::cout << entry.path().string() << " => ";
    if (ham_score > spam_score) {
      //  std::cout << "HAM\n";
      ham_count++;
    } else {
      // std::cout << "SPAM\n";
      spam_count++;
    }
  }

  std::cout << "\n";
  std::cout << "\n";
  std::cout << "RESULT: \n";

  if (ham_count > spam_count) {
    std::cout << "probability of ham => "
              << float(ham_count) / float(ham_count + spam_count) << std::endl;

    std::cout << "probability of spam => "
              << float(spam_count) / float(ham_count + spam_count) << std::endl;
  } else {
    std::cout << "probability of spam => "
              << float(spam_count) / float(ham_count + spam_count) << std::endl;

    std::cout << "probability of ham => "
              << float(ham_count) / float(ham_count + spam_count) << std::endl;
  }
}
