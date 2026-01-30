#include <iostream>
#include <string>
#include "lib/process_lib.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: test_util <command> <mode>" << std::endl;
        std::cout << "Modes:" << std::endl;
        std::cout << "  --bg    - Run in background" << std::endl;
        std::cout << "  --wait  - Run and wait for exit code" << std::endl;
        return 1;
    }

    std::string cmd = argv[1];
    std::string mode = argv[2];
    bool wait_mode = false;

    if (mode == "--wait") {
        wait_mode = true;
    } else if (mode == "--bg") {
        wait_mode = false;
    } else {
        std::cout << "Unknown mode: " << mode << std::endl;
        return 1;
    }

    std::cout << "Launching: " << cmd << " (mode: " << mode << ")" << std::endl;

    ProcessResult res = run_program(cmd, wait_mode);

    if (res.success) {
        if (wait_mode) {
            std::cout << "Program finished with exit code: " << res.exit_code << std::endl;
        } else {
            std::cout << "Program is now running in background." << std::endl;
        }
    } else {
        std::cerr << "Error: Could not launch the program." << std::endl;
        return 1;
    }

    return 0;
}