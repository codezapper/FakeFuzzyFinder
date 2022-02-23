#pragma once

#include "generic.h"

class Matcher {
	private:
		std::map<std::string, int> score_map;

	public:
        Matcher() {

        }
        std::vector<std::string>get_matches(std::string user_input, std::vector<std::string> files_list, int lines=DEFAULT_LINES);
        int compute_score(std::string item, std::string user_input);
};
