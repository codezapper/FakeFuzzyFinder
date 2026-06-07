#include "term.h"

#include <cassert>
#include <iostream>
#include <string>

// The maximum number of characters a displayed line may occupy.
static const int MAX_WIDTH = 80;

int main() {
	// A line far longer than the terminal width must be shortened so the
	// amount of displayed characters never goes over the width.
	std::string long_line(200, 'a');
	std::string out = truncate_middle(long_line, MAX_WIDTH);
	assert((int)out.size() <= MAX_WIDTH);
	assert((int)out.size() == MAX_WIDTH);

	// The middle is replaced with exactly three dots.
	assert(out.find("...") != std::string::npos);
	assert(out.find("....") == std::string::npos);

	// Both ends of the original text are preserved around the dots.
	assert(out.front() == 'a');
	assert(out.back() == 'a');

	// A realistic, overly long path is truncated in the middle while keeping
	// the head and the (more informative) tail.
	std::string path =
		"/very/long/path/that/keeps/going/and/going/until/it/is/way/too/long/to/fit/file.txt";
	assert((int)path.size() > MAX_WIDTH);
	std::string tpath = truncate_middle(path, MAX_WIDTH);
	assert((int)tpath.size() <= MAX_WIDTH);
	assert(tpath.find("...") != std::string::npos);
	assert(tpath.substr(0, 5) == "/very");
	assert(tpath.substr(tpath.size() - 8) == "file.txt");

	// A line that already fits is returned untouched.
	std::string short_line = "already short";
	assert(truncate_middle(short_line, MAX_WIDTH) == short_line);

	// A line exactly at the width is left unchanged (no truncation needed).
	std::string exact(MAX_WIDTH, 'b');
	std::string texact = truncate_middle(exact, MAX_WIDTH);
	assert((int)texact.size() == MAX_WIDTH);
	assert(texact == exact);

	std::cout << "All truncate_middle tests passed." << std::endl;
	return 0;
}
