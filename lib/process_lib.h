#ifndef PROCESS_LIB_H
#define PROCESS_LIB_H

#include <string>

struct ProcessResult {
    int exit_code;
    bool success;
};

ProcessResult run_program(const std::string& command, bool wait);

#endif