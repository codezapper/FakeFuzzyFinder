#include "term.h"

int match_done;

bool is_printable(char c) {
	if ((c >= 0x20) && (c <= 0x7E)) {
		return true;
	}

	return false;
}

bool is_extended(char c) {
	return (c == 0x1B);
}

TermHandler::TermHandler() {
}

void TermHandler::init() {
	freopen("/dev/tty", "r", stdin);

	std::wcout << std::unitbuf;

	struct termios term;
	tcgetattr(0, &this->_original_term);
	tcgetattr(0, &term);
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &term);
	setbuf(stdin, NULL);
	this->_initialized = true;
}

void TermHandler::reset() {
	if (this->_initialized) {
		tcsetattr(0, TCSANOW, &this->_original_term);
	}
	this->_initialized = false;
}

int TermHandler::kbhit() {
	static int nbbytes;
	ioctl(0, FIONREAD, &nbbytes);
	return nbbytes;
}

std::string TermHandler::handle_input(std::string user_input, int list_size, int &selected_index) {
	char c = getchar();

	if (is_extended(c)) {
		c = getchar();
		// Discard the first one
		c = getchar();

		switch (c) {
			case 0x41: selected_index = this->go_up(selected_index, list_size); break;
			case 0x42: selected_index = this->go_down(selected_index, list_size); break;
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

void TermHandler::clear_output(int lines) {
	// Clear current cursor line (for input)
	std::cout << "\33[2K";
	for (int i = 0; i < lines; i++) {
		std::cout << "\33[A";
		std::cout << "\33[2K";
		std::cout << "\r";
	}
}

int TermHandler::go_down(int index, int lines) {
	if (index < (lines - 1)) {
		index++;
	}

	return index;
}

int TermHandler::go_up(int index, int lines) {
	if (index > 0) {
		index--;
	}

	return index;
}

void TermHandler::show_matches(std::vector<std::string>matches_list, int selected_index, int lines) {
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
