#include "fullreader.h"

std::string FullReader::read_it() {
	std::string item;
	std::vector<std::string> items_list;
	std::string user_input;
	std::string prev_user_input;
	char c;

	while (std::getline(std::cin, item)) {
		items_list.push_back(item);
	}

    this->term->init();

	std::vector<std::string> matches_list;
	std::vector<std::string> prev_matches_list;
	int selected_index = 0;
	int prev_index = -1;
	Matcher matcher = Matcher();

	// TODO: Using a signal would be better
    // That would allow to have separate threads
    // for updating the display, reading user input
    // and (when streaming) updating the list of items
	while (!match_done) {
		// Only recompute matches if user_input changed
		if (prev_user_input != user_input) {
			matches_list = matcher.get_matches(user_input, items_list);
			prev_user_input = user_input;
			prev_index = -1;  // Force redraw after match change
		}
		
		// Only redraw if matches or selection changed
		if ((prev_matches_list != matches_list) || (prev_index != selected_index)) {
			prev_matches_list = matches_list;
			prev_index = selected_index;
			this->term->clear_output();
			this->term->show_matches(matches_list, selected_index);
		}
		
		std::cout << user_input.c_str();
		fflush(stdout);

		while (!this->term->kbhit()) {
		}

		user_input = this->term->handle_input(user_input, matches_list.size(), selected_index);
	}

	this->term->clear_output();
    this->term->reset();

    return matches_list[selected_index];
}
