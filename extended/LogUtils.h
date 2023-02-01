#pragma once

#include <cstdio>

template<typename... Args>
void print_verbose(bool verbose, Args... args) {
    if (verbose) {
        printf(args...);
    }
}
