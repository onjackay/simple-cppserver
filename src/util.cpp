#include "util.h"
#include <iostream>

void errif(int cond, const char *msg) {
    if (cond) {
        std::cerr << msg << std::endl;
        exit(EXIT_FAILURE);
    }
}