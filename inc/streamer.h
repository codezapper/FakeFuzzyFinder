#pragma once

#include "generic.h"
#include "term.h"
#include "input.h"
#include "matcher.h"

extern int match_done;

class Streamer {
    private:
        std::string shared_item;
        TermHandler *term;

    public:
        Streamer(TermHandler *term) {
            this->term = term;
        }
        void match_it(std::string &selected_value);
        std::string stream_it(std::string cmd);
};
