#include "term.h"

struct termios original_term;
extern int match_done;

void init_terminal_input() {
	std::wcout << std::unitbuf;

	struct termios term;
	tcgetattr(0, &original_term);
	tcgetattr(0, &term);
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &term);
	setbuf(stdin, NULL);
}

int kbhit() {
	static int nbbytes;
	ioctl(0, FIONREAD, &nbbytes);
	return nbbytes;
}

std::string handle_input(std::string user_input, int list_size, int &selected_index) {
	char c = getchar();

	if (is_extended(c)) {
		c = getchar();
		// Discard the first one
		c = getchar();

		switch (c) {
			case 0x41: selected_index = go_up(selected_index, list_size); break;
			case 0x42: selected_index = go_down(selected_index, list_size); break;
		}
	} else {
		if (is_printable(c)) {
			user_input += c;
		} else {
			if (0x7F == c) {
				user_input.pop_back();
			} else if ((0x09 == c) || (0x0D == c) || (0x0A == c)) {
				match_done = 1;
			}
		}
	}

	return user_input;
}

bool is_printable(char c) {
	if ((c >= 0x20) && (c <= 0x7E)) {
		return true;
	}

	return false;
}

void clear_output(int lines) {
	// Clear current cursor line (for input)
	std::cout << "\33[2K";
	for (int i = 0; i < lines; i++) {
		std::cout << "\33[A";
		std::cout << "\33[2K";
		std::cout << "\r";
	}
}

bool is_extended(char c) {
	return (c == 0x1B);
}

int go_down(int index, int lines) {
	if (index < (lines - 1)) {
		index++;
	}

	return index;
}

int go_up(int index, int lines) {
	if (index > 0) {
		index--;
	}

	return index;
}

void show_matches(std::vector<std::string>matches_list, int selected_index, int lines) {
	int index = 0;
	for (auto it = std::begin(matches_list); it != std::end(matches_list); ++it) {
		if (index == selected_index) {
			std::cout << "\33[7m" << *it << "\033[0m" << std::endl;
		} else {
			std::cout << *it << std::endl;
		}
		index++;
	}

	for (int i = 0; i < (lines - matches_list.size()); i++) {
		std::cout << std::endl;
	}
}

