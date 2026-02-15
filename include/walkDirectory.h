
#ifndef GET_TOKEN_H
#define GET_TOKEN_H
#include <iostream>
#include <unordered_map>
#include <utility>

std::pair<int, int> walkDirectory(std::string &path,
                                  std::unordered_map<std::string, int> &spam,
                                  std::unordered_map<std::string, int> &ham);
#endif
