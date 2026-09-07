#include "input.h"
#include <memory>

std::string get_command() {
	const char *cmd = std::getenv("FFF_COMMAND");

	if (cmd != NULL) {
		return std::string(cmd);
	}

	return std::string("");
}

std::string get_items_from_command(const char *cmd) {
	static bool finished = false;
	static std::unique_ptr<FILE, decltype(&pclose)> pipe(nullptr, pclose);

	if (finished) {
		return std::string(SENTINEL_STRING);
	}

	// Initialize pipe only once, on first call
	if (!pipe) {
		pipe.reset(popen(cmd, "r"));
		if (!pipe) {
			throw std::runtime_error("popen() failed!");
		}
	}

	std::array<char, 128> buffer;
	std::string result;

	if (fgets(buffer.data(), buffer.size(), pipe.get()) != NULL) {
		return buffer.data();
	}

	finished = true;
	pipe.reset();  // Explicitly close
	return std::string(SENTINEL_STRING);
}

FILE *open_out_file(int argc, char **argv) {
	FILE *out_file = NULL;
	if (argc > 1) {
		out_file = fopen(argv[1], "r");
		if (out_file != NULL) {
			fclose(out_file);
			exit(1);
		}

		out_file = fopen(argv[1], "w");
		if (out_file == NULL) {
			exit(1);
		}
	}

	return out_file;
}

void close_out_file(FILE *out_file, std::string selected_value) {
	if (out_file == NULL) {
		std::cout << std::endl << selected_value;
	} else {
		fprintf(out_file, "%s", selected_value.c_str());
		fclose(out_file);
	}
}
