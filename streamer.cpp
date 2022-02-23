#include "streamer.h"

extern std::mutex update_mtx;
extern std::string shared_item;
extern struct termios original_term;
extern int match_done;

void match_it(std::string &selected_value, TermHandler *term) {
	std::vector<std::string> items_list;
	std::vector<std::string> matches_list;
	std::vector<std::string> prev_matches_list;

	std::string user_input;
	std::string prev_user_input;

	bool first_show = true;
	int selected_index = 0;
	int prev_index = 0;
	char c;

	freopen("/dev/tty", "r", stdin);
	term->init();

	bool must_compute = true;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	while (!match_done) {
		while (!term->kbhit()) {
			if (shared_item == "") {
				continue;
			}
			if (shared_item != SENTINEL_STRING) {
				update_mtx.lock();
				items_list.push_back(shared_item);
				update_mtx.unlock();
				shared_item = SENTINEL_STRING;
				must_compute = true;
			}

			if (prev_user_input != user_input) {
				must_compute = true;
			}

			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

			if ((must_compute) && (interval > 1)) {
				matches_list = get_matches(user_input, items_list);
				must_compute = false;
				begin = std::chrono::steady_clock::now();
			}

			if ((prev_matches_list != matches_list) || (prev_index != selected_index) || (prev_user_input != user_input)) {
				prev_matches_list = matches_list;
				prev_index = selected_index;
				prev_user_input = user_input;
				if (!first_show) {
					term->clear_output();
				}
				term->show_matches(matches_list, selected_index);
				first_show = false;
				std::cout << user_input;
			}
			fflush(stdout);
		}

		user_input = term->handle_input(user_input, matches_list.size(), selected_index);
	}

	//clear_output();
	printf("\n%s", matches_list[selected_index].c_str());
	selected_value = matches_list[selected_index];
	term->reset();
}

std::string stream_it(std::string cmd, TermHandler *term) {
	std::string selected_value;
	std::thread match_thread(match_it, std::ref(selected_value), term);
	std::string item;

	while ((item = get_items_from_command(cmd.c_str())) != SENTINEL_STRING) {
		item.erase(std::remove(item.begin(), item.end(), '\n'), item.end());
		if (item == "") {
			continue;
		}

		update_mtx.lock();
		shared_item = item;
		update_mtx.unlock();

		while (shared_item != SENTINEL_STRING) {
			if (match_done == 1) {
				break;
			}
		}

		if (match_done == 1) {
			break;
		}

	}

	match_thread.join();
	return selected_value;
}

