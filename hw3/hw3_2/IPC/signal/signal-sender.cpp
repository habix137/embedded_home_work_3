#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <PID> <signal_number>\n";
        return 1;
    }

    pid_t target = static_cast<pid_t>(std::stoi(argv[1]));
    int sig = std::stoi(argv[2]);

    if (kill(target, sig) == -1) {
        std::cerr << "Error sending signal: " << std::strerror(errno) << "\n";
        return 1;
    }

    std::cout << "Sent signal " << sig
              << " (" << strsignal(sig) << ") to PID " << target << "\n";
    return 0;
}
