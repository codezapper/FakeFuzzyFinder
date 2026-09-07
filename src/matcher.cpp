#include "matcher.h"
#include <queue>
#include <cctype>

static char normalize_char(char c) {
	if ((c >= 0x41) && (c <= 0x5A)) {
		return c + 0x20;
	}
	return c;
}

int Matcher::compute_score(std::string item, std::string user_input) {
	int current_score = 1000;
	int current_multiplier = 1;
	int total_score = 0;

	bool found = false;
	int j = 0;
	int prev_j = 0;

	for (int i = 0; i < user_input.size(); i++) {
		found = false;
		int start_j = j;
		char user_char = normalize_char(user_input[i]);
		
		while (j < item.size()) {
			char item_char = normalize_char(item[j]);

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
	// Priority queue: max-heap of (score, item)
	auto cmp = [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
		return a.first > b.first;  // Min-heap (we want min scores to be removed first)
	};
	std::priority_queue<std::pair<int, std::string>, 
	                   std::vector<std::pair<int, std::string>>,
	                   decltype(cmp)> pq(cmp);

	if (user_input == "") {
		if (items_list.size() > lines) {
			std::vector<std::string>limited(items_list.begin(), items_list.begin() + lines);
			return limited;
		}
		return items_list;
	}

	int min_score = -999;

	for (auto it = std::begin(items_list); it != std::end(items_list); ++it) {
		int score = -2;
		std::string map_key = user_input + (*it);
		
		if (score_map.find(map_key) == score_map.end() || score_map[map_key] == 0) {
			score_map[map_key] = compute_score((*it), user_input);
		}

		score = score_map[map_key];
		
		if (score > -1) {
			if ((int)pq.size() < lines) {
				pq.push({score, *it});
				if (score < min_score) {
					min_score = score;
				}
			} else if (score > pq.top().first) {
				pq.pop();
				pq.push({score, *it});
			}
		}
	}

	// Extract results from priority queue in descending order
	std::vector<std::pair<int, std::string>> temp_results;
	while (!pq.empty()) {
		temp_results.push_back(pq.top());
		pq.pop();
	}
	
	// Reverse to get descending order
	std::vector<std::string> matches;
	for (auto it = temp_results.rbegin(); it != temp_results.rend(); ++it) {
		matches.push_back(it->second);
	}

	return matches;
}
