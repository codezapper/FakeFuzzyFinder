#pragma once

#include "generic.h"

std::vector<std::string>get_matches(std::string user_input, std::vector<std::string> files_list, int lines=DEFAULT_LINES);
bool compare_scores(std::pair<std::string, int> i, std::pair<std::string, int> j);
std::pair<std::string, int> get_best(std::map<std::string, int> score_map);
bool string_matches(std::string pattern, std::string file_name);
int compute_score(std::string item, std::string user_input);
