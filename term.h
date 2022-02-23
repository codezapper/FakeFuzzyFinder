#pragma once

#include "generic.h"

class TermHandler {
	private:
		struct termios _original_term;
		bool _initialized = false;

	public:
		TermHandler();
		void init();
		void reset();
		int kbhit();
		std::string handle_input(std::string user_input, int list_size, int &selected_index);
		void clear_output(int lines=DEFAULT_LINES);
        int go_down(int index, int lines);
        int go_up(int index, int lines);
        void show_matches(std::vector<std::string>matches_list, int selected_index, int lines=DEFAULT_LINES);
};
