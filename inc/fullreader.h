#pragma once

#include "matcher.h"
#include "term.h"

extern int match_done;

class FullReader {
    private:
        TermHandler *term;

    public:
        FullReader(TermHandler *term) {
            this->term = term;
        }
        std::string read_it();
};
