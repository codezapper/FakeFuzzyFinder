#include "generic.h"

std::string get_command();
std::string get_items_from_command(const char * cmd);
FILE *open_out_file(int argc, char **argv);
void close_out_file(FILE *out_file, std::string selected_value);
