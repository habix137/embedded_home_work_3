#pragma once
#include "ssh_connection.h"
#include <optional>

/* ----- one–shot helpers ----- */
bool  query_basic_specs (SSHConnection&, std::string& pretty);
bool  query_usage       (SSHConnection&, double& cpu, double& ram);
bool  query_top_process (SSHConnection&, int& pid,
                         std::string& name, double& mem_pct);
bool  kill_process      (SSHConnection&, int pid);

/* ----- high-level loop (30× every 30 s) ----- */
void  monitor_loop(SSHConnection& conn,
                   double cpu_threshold,
                   double ram_threshold,
                   int    iterations = 10,
                   int    interval_s = 2);
