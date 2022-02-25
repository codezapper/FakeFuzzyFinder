#include "streamer.h"

std::mutex update_mtx;

void Streamer::match_it(std::string &selected_value) {
	std::vector<std::string> items_list;
	std::vector<std::string> matches_list;
	std::vector<std::string> prev_matches_list;

	std::string user_input;
	std::string prev_user_input;

	bool first_show = true;
	int selected_index = 0;
	int prev_index = 0;

	this->term->init();

	bool must_compute = true;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	Matcher matcher = Matcher();

	// TODO: Using a signal would be better
    // That would allow to have separate threads
    // for updating the display, reading user input
    // and (when streaming) updating the list of items
	while (!match_done) {
		while (!this->term->kbhit()) {
			if (this->shared_item == "") {
				continue;
			}
			if (this->shared_item != SENTINEL_STRING) {
				update_mtx.lock();
				items_list.push_back(this->shared_item);
				update_mtx.unlock();
				this->shared_item = SENTINEL_STRING;
				must_compute = true;
			}

			if (prev_user_input != user_input) {
				must_compute = true;
			}

			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

			if ((must_compute) && (interval > 1)) {
				matches_list = matcher.get_matches(user_input, items_list);
				must_compute = false;
				begin = std::chrono::steady_clock::now();
			}

			if ((prev_matches_list != matches_list) || (prev_index != selected_index) || (prev_user_input != user_input)) {
				prev_matches_list = matches_list;
				prev_index = selected_index;
				prev_user_input = user_input;
				if (!first_show) {
					this->term->clear_output();
				}
				this->term->show_matches(matches_list, selected_index);
				first_show = false;
				std::cout << user_input;
			}
			fflush(stdout);
		}

		user_input = this->term->handle_input(user_input, matches_list.size(), selected_index);
	}

	//clear_output();
	std::cout << std::endl << matches_list[selected_index];
	selected_value = matches_list[selected_index];
	this->term->reset();
}

std::string Streamer::stream_it(std::string cmd) {
	std::string selected_value;
	std::thread match_thread(&Streamer::match_it, this, std::ref(selected_value));
	std::string item;

	while ((item = get_items_from_command(cmd.c_str())) != SENTINEL_STRING) {
		item.erase(std::remove(item.begin(), item.end(), '\n'), item.end());
		if (item == "") {
			continue;
		}

		update_mtx.lock();
		this->shared_item = item;
		update_mtx.unlock();

		while (this->shared_item != SENTINEL_STRING) {
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

