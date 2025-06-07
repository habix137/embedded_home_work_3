#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <fstream>
#include <cstring>

// logfile is opened at startup in append mode
static std::ofstream log_file;

void signal_handler(int signo) {
    if (!log_file.is_open()) return;
    log_file << "Received signal: " << signo << " (" << strsignal(signo) << ")\n"
             << std::flush;
}

int main() {
    // 1) Print our PID
    pid_t pid = getpid();
    std::cout << "Signal receiver PID = " << pid << "\n";

    // 2) Open the log file
    log_file.open("signals.log", std::ios::app);
    if (!log_file) {
        std::cerr << "Error: cannot open signals.log for writing\n";
        return 1;
    }

    // 3) Install the handler for all catchable signals
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Skip SIGKILL(9) and SIGSTOP(19) since they cannot be caught
    for (int s = 1; s < NSIG; ++s) {
        if (s == SIGKILL || s == SIGSTOP) continue;
        sigaction(s, &sa, nullptr);
    }

    // 4) Wait forever
    std::cout << "Waiting for signals… (logs to signals.log)\n";
    while (true) {
        pause();  // sleep until a signal arrives
    }

    return 0;
}
