#include "matcher.h"
#include <ctime>


int Matcher::compute_score(std::string item, std::string user_input) {
	int current_score = 1000;
	int current_multiplier = 1;
	int total_score = 0;

	bool found = false;
	int j = 0;
	int prev_j = 0;

	for (int i = 0; i < user_input.size(); i++) {
		found = false;
		int start_j = i;
		while (j < item.size()) {
			char user_char = user_input[i];
			char item_char = item[j];

			if ((user_char >= 0x41) && (user_char <= 0x5A)) {
				user_char += 0x20;
			}

			if ((item_char >= 0x41) && (item_char <= 0x5A)) {
				item_char += 0x20;
			}

			if (user_char == item_char) {
				found = true;

				if (j == start_j) {
					current_multiplier += 50;
				} else {
					current_multiplier -= 5;
				}

				current_score += (10 * current_multiplier);
				j++;
				break;
			}
			j++;
		}

		if (!found) {
			return -1;
		}
	}
	return current_score;
}

std::vector<std::string> Matcher::get_matches(std::string user_input, std::vector<std::string> items_list, int lines) {
	std::map<std::string, int>match_map;
	int min_score = -999;
	std::string min_string = "";

	if (user_input == "") {
		if (items_list.size() > lines) {
			std::vector<std::string>limited(items_list.begin(), items_list.begin() + lines);
			return limited;
		}
	}

	for (auto it = std::begin(items_list); it != std::end(items_list); ++it) {
		int score = -2;
		std::string map_key = user_input + (*it);
		if (score_map[map_key] == 0) {
			score_map[map_key] = compute_score((*it), user_input);
		}

		score = score_map[map_key];
		if ((match_map.size() < lines) && (score > -1)) {
			match_map[*it] = score;
			if (min_score < score) {
				min_score = score;
				min_string = *it;
			}
		} else {
			if ((score > -1) && (min_score < score)) {
				match_map.erase(min_string);
				match_map[*it] = score;
				for (auto it_min = match_map.begin(); it_min != match_map.end(); ++it_min) {
					if (it_min->second <= min_score) {
						min_score = it_min->second;
						min_string = it_min->first;
					}
				}
			}
		}
	}

	int cnt = 0;
	std::vector<std::string> matches;

	while (match_map.size() > 0) {
		std::map<std::string, int>::iterator it_max;
		int max_score = -2;
		std::string max_str;
		for (auto it = match_map.begin(); it != match_map.end(); ++it) {
			if (max_score <= it->second) {
				max_str = it->first;
				max_score = it->second;
			}
		}

		matches.push_back(max_str);
		match_map.erase(max_str);

		cnt++;
		if (cnt >= lines) {
			break;
		}
	}

	return matches;
}
