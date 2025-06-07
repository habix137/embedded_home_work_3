#include "ssh_connection.h"
#include <iostream>

SSHConnection::SSHConnection(std::string h, std::string u,
                             std::string p, int prt)
    : host_(std::move(h)), user_(std::move(u)),
      pass_(std::move(p)),  port_(prt) {}

SSHConnection::~SSHConnection() {
    if (sess_) { ssh_disconnect(sess_); ssh_free(sess_); }
}

bool SSHConnection::open() {
    sess_ = ssh_new();
    if (!sess_) return false;

    ssh_options_set(sess_, SSH_OPTIONS_HOST,  host_.c_str());
    ssh_options_set(sess_, SSH_OPTIONS_USER,  user_.c_str());
    ssh_options_set(sess_, SSH_OPTIONS_PORT, &port_);

    if (ssh_connect(sess_) != SSH_OK) {
        std::cerr << "Connect: " << ssh_get_error(sess_) << '\n';
        return false;
    }
    if (ssh_userauth_password(sess_, nullptr, pass_.c_str()) != SSH_AUTH_SUCCESS) {
        std::cerr << "Auth: " << ssh_get_error(sess_) << '\n';
        return false;
    }
    return true;
}

bool SSHConnection::exec(const std::string& cmd,
                         std::string& out) const
{
    ssh_channel ch = ssh_channel_new(sess_);
    if (!ch) return false;

    if (ssh_channel_open_session(ch) != SSH_OK ||
        ssh_channel_request_exec(ch, cmd.c_str()) != SSH_OK) {
        ssh_channel_close(ch); ssh_channel_free(ch); return false;
    }

    char buf[256]; int n;
    while ((n = ssh_channel_read(ch, buf, sizeof buf, 0)) > 0)
        out.append(buf, n);

    ssh_channel_send_eof(ch);
    ssh_channel_close(ch);
    ssh_channel_free(ch);
    return true;
}
