#include "hardware_monitor.h"
#include <fmt/core.h>           // comment out if you switched to <iostream>
#include <chrono>
#include <iostream>
#include <thread>
#include <sstream>

/* -----------  helper to trim trailing newline  ----------- */
static inline void rtrim(std::string& s)
{
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r'))
        s.pop_back();
}

/* -----------  basic specs once at start  ----------- */
bool query_basic_specs(SSHConnection& c, std::string& pretty)
{
    const char* cmd =
        "echo \"CPU: $(grep -m1 'model name' /proc/cpuinfo | cut -d: -f2-)\";"
        "echo \"RAM: $(free -h | awk '/Mem:/ {print $2}')\"";

    if (!c.exec(cmd, pretty)) return false;
    rtrim(pretty);
    return true;
}

/* -----------  live usage  ----------- */
bool query_usage(SSHConnection& c, double& cpu, double& ram)
{
    std::string out;

    /*  CPU  = user + system  (top -bn1)           -> e.g.  6.4
        RAM% = (MemTotal-MemAvailable)/MemTotal*100 -> e.g. 91.3     */
    const char* cmd =
        "CPU=$(top -bn1 | grep 'Cpu(s)' | awk '{print $2+$4}');"
        "RAM=$(awk '/MemTotal/ {t=$2} /MemAvailable/ {a=$2} "
        "           END {printf \"%.1f\", 100*(t-a)/t}' /proc/meminfo);"
        "echo \"$CPU $RAM\"";

    if (!c.exec(cmd, out)) return false;

    std::istringstream iss(out);
    return bool(iss >> cpu >> ram);
}

/* -----------  top memory hog  ----------- */
bool query_top_process(SSHConnection& c,
                       int&           pid,
                       std::string&   name,
                       double&        mem)
{
    std::string out;
    const char* cmd =
        "ps -eo pid,comm,%mem --sort=-%mem | "
        "awk 'NR==2 {print $1, $2, $3}'";

    if (!c.exec(cmd, out)) return false;
    std::istringstream iss(out);
    return bool(iss >> pid >> name >> mem);
}

/* -----------  killer  ----------- */
bool kill_process(SSHConnection& c, int pid)
{
    std::string dummy;
    std::string cmd = "kill -9 " + std::to_string(pid);
    return c.exec(cmd, dummy);
}

/* -----------  monitor loop  ----------- */
void monitor_loop(SSHConnection& conn,
                  double cpu_thr,
                  double ram_thr,
                  int    iterations,
                  int    interval_s)
{
    for (int i = 0; i < iterations; ++i)
    {
        double cpu{}, ram{};
        if (!query_usage(conn, cpu, ram)) {
            std::cerr << "Failed to read usage\n";
            break;
        }

        std::cout << fmt::format("CPU {:>5.1f}%  RAM {:>5.1f}%\n",
                                 cpu, ram);

        if (cpu > cpu_thr || ram > ram_thr)
        {
            std::cout << "*** Threshold exceeded! "
                      << "(CPU>" << cpu_thr << " || RAM>" << ram_thr << ")\n";

            int pid; std::string name; double mem;
            if (query_top_process(conn, pid, name, mem)) {
                std::cout << "   Top offender → PID " << pid
                          << " (" << name << ")  MEM " << mem << "%\n";
                if (kill_process(conn, pid))
                    std::cout << "   → Process killed.\n";
                else
                    std::cout << "   → Failed to kill process.\n";
            }
            break;          // stop monitoring after intervention
        }

        std::this_thread::sleep_for(std::chrono::seconds(interval_s));
    }
}
