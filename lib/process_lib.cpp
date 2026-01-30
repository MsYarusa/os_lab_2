#include "process_lib.h"
#include <iostream>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/wait.h>
#endif

ProcessResult run_program(const std::string& command, bool wait) {
    ProcessResult result = {-1, false};

#ifdef _WIN32
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, (char*)command.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "[ERROR] CreateProcess failed! System Error Code: " << GetLastError() << std::endl;
        return result;
    }

    result.success = true;

    if (wait) {
        DWORD wait_result = WaitForSingleObject(pi.hProcess, INFINITE);
        
        if (wait_result == WAIT_OBJECT_0) {
        } else {
            std::cout << "[DEBUG] Wait failed with code: " << wait_result << std::endl;
        }

        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        result.exit_code = static_cast<int>(exitCode);
    } else {
        result.exit_code = 0;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

#else
    pid_t pid = fork();

    if (pid < 0) {
        return result;
    }

    if (pid == 0) { 
        execl("/bin/sh", "sh", "-c", command.c_str(), (char*)NULL);
        _exit(127);
    } else {
        result.success = true;
        if (wait) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                result.exit_code = WEXITSTATUS(status);
            }
        } else {
            result.exit_code = 0;
        }
    }
#endif

    if (result.success) {
        std::cout << "[SYSTEM] Finishing task" << std::flush;
        
        for (int i = 0; i < 3; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "." << std::flush;
        }
        
        std::cout << " DONE!" << std::endl;
    }

    return result;
}