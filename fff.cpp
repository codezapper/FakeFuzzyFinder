#include "matcher.h"
#include "streamer.h"

std::mutex update_mtx;
std::string shared_item;

extern struct termios original_term;
int match_done = 0;


int main(int argc, char **argv) {
	std::string file_name;
	std::vector<std::string> items_list;
	std::string user_input;
	char c;

	FILE *out_file = open_out_file(argc, argv);
	TermHandler term = TermHandler();

	std::string cmd = get_command();

	if (cmd != "") {
		std::string selected_value = stream_it(cmd, &term);
		close_out_file(out_file, selected_value);
		return 0;
	}

	while (std::getline(std::cin, file_name)) {
		items_list.push_back(file_name);
	}

	freopen("/dev/tty", "r", stdin);

	term.init();
	std::vector<std::string> matches_list;

	int selected_index = 0;

	while (!match_done) {
		matches_list = get_matches(user_input, items_list);
		show_matches(matches_list, selected_index);
		std::cout << user_input.c_str();
		fflush(stdout);

		while (!term.kbhit()) {
		}

		user_input = term.handle_input(user_input, items_list.size(), selected_index);
		term.clear_output();
	}

	term.clear_output();
	close_out_file(out_file, matches_list[selected_index]);
	term.reset();

	return 0;
}

