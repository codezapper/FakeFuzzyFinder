#pragma once

#include "generic.h"
#include "term.h"
#include "input.h"
#include "matcher.h"

void match_it(std::string &selected_value, TermHandler *term);
std::string stream_it(std::string cmd, TermHandler *term);
