#include "streamer.h"
#include <thread>
#include <condition_variable>

std::mutex update_mtx;
std::condition_variable item_ready_cv;
std::condition_variable item_consumed_cv;

void Streamer::match_it(std::string &selected_value) {
	std::vector<std::string> items_list;
	std::vector<std::string> matches_list;
	std::vector<std::string> prev_matches_list;

	std::string user_input;
	std::string prev_user_input;

	bool first_show = true;
	int selected_index = 0;
	int prev_index = -1;

	this->term->init();

	bool must_compute = true;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	Matcher matcher = Matcher();

	while (!match_done) {
		// Check if new item arrived (with timeout to avoid complete blocking)
		{
			std::unique_lock<std::mutex> lock(update_mtx);
			if (this->shared_item != "" && this->shared_item != SENTINEL_STRING) {
				items_list.push_back(this->shared_item);
				this->shared_item = SENTINEL_STRING;
				must_compute = true;
				item_consumed_cv.notify_one();  // Wake up producer
			}
		}

		if (prev_user_input != user_input) {
			must_compute = true;
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

		// Only recompute if enough time has passed AND something changed
		if ((must_compute) && (interval > 1)) {
			matches_list = matcher.get_matches(user_input, items_list);
			prev_user_input = user_input;
			must_compute = false;
			begin = std::chrono::steady_clock::now();
		}

		// Only redraw if results or selection changed
		if ((prev_matches_list != matches_list) || (prev_index != selected_index) || (prev_user_input != user_input)) {
			prev_matches_list = matches_list;
			prev_index = selected_index;
			if (!first_show) {
				this->term->clear_output();
			}
			this->term->show_matches(matches_list, selected_index);
			first_show = false;
		}
		
		std::cout << user_input;
		fflush(stdout);
		
		// Small sleep to prevent 100% CPU usage
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

		{
			std::unique_lock<std::mutex> lock(update_mtx);
			this->shared_item = item;
			item_ready_cv.notify_one();  // Wake up consumer
		}

		// Wait for item to be consumed with timeout (max 100ms)
		{
			std::unique_lock<std::mutex> lock(update_mtx);
			if (!item_consumed_cv.wait_for(lock, std::chrono::milliseconds(100), 
			                               [this]() { return this->shared_item == SENTINEL_STRING; })) {
				// Timeout - consumer might be blocked, check if match_done
				if (match_done == 1) {
					break;
				}
			}
		}

		if (match_done == 1) {
			break;
		}
	}

	match_thread.join();
	return selected_value;
}
