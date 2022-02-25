#include "streamer.h"
#include "fullreader.h"

int main(int argc, char **argv) {
	FILE *out_file = open_out_file(argc, argv);
	TermHandler term = TermHandler();

	std::string cmd = get_command();
	std::string selected_value;

	if (cmd != "") {
		Streamer streamer = Streamer(&term);
		selected_value = streamer.stream_it(cmd);
	} else {
		FullReader reader = FullReader(&term);
		selected_value = reader.read_it();
	}

	close_out_file(out_file, selected_value);
}
