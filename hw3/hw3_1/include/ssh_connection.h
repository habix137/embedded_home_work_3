#pragma once
#include <libssh/libssh.h>
#include <string>

/* A tiny RAII wrapper around libssh. */
class SSHConnection {
public:
    SSHConnection(std::string host,
                  std::string user,
                  std::string password,
                  int         port = 22);
    ~SSHConnection();

    bool open();                                    // connect + authenticate
    bool exec(const std::string& cmd,
              std::string& stdout_buf) const;       // run remote cmd

private:
    ssh_session  sess_{nullptr};
    std::string  host_, user_, pass_;
    int          port_;
};
