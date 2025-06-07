#include "ssh_connection.h"
#include "hardware_monitor.h"
#include <iostream>

int main(int argc, char* argv[])
{
    /* ---- simple CLI: host user pass cpu% ram% ---- */
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0]
                  << " <host> <user> <pass> <cpu_thr%> <ram_thr%>\n";
        return 1;
    }
    const std::string host  = argv[1];
    const std::string user  = argv[2];
    const std::string pass  = argv[3];
    const double cpu_thr = std::stod(argv[4]);
    const double ram_thr = std::stod(argv[5]);

    SSHConnection conn{host, user, pass};
    if (!conn.open()) return 1;

    std::string specs;
    if (query_basic_specs(conn, specs))
        std::cout << "Remote specs:\n" << specs << "\n\n";

    monitor_loop(conn, cpu_thr, ram_thr);
}
