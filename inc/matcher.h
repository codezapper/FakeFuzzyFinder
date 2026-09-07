#pragma once

#include "generic.h"

class Matcher {
	private:
		std::map<std::string, int> score_map;

	public:
        Matcher() {

        }
        
        // Clear score cache when appropriate (call between major search changes)
        void clear_cache() {
            score_map.clear();
        }
        
        std::vector<std::string>get_matches(std::string user_input, std::vector<std::string> files_list, int lines=DEFAULT_LINES);
        int compute_score(std::string item, std::string user_input);
};
