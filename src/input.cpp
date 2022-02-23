#include "input.h"

std::string get_command() {
	const char *cmd = std::getenv("FFF_COMMAND");

	if (cmd != NULL) {
		return std::string(cmd);
	}

	return std::string("");
}

std::string get_items_from_command(const char *cmd) {
	static bool finished = false;
	static std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

	if (finished) {
		return std::string(SENTINEL_STRING);
	}

	std::array<char, 128> buffer;
	std::string result;
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}

	if (fgets(buffer.data(), buffer.size(), pipe.get()) != NULL) {
		return buffer.data();
	}

	finished = true;
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

		fclose(out_file);

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

