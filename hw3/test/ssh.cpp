#include <libssh/libssh.h>
#include <iostream>
#include <cstdlib>

int main() {
    ssh_session my_ssh_session;
    int rc;
    char *password;
    ssh_channel channel;
    
    // SSH connection details
    const char* host = "192.168.1.30";
    const char* username = "orangepi";
    const char* command = "ls -l";

    // Create SSH session
    my_ssh_session = ssh_new();
    if (my_ssh_session == nullptr) {
        std::cerr << "Error creating SSH session" << std::endl;
        return EXIT_FAILURE;
    }

    // Set SSH options
    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, username);

    // Connect to server
    rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK) {
        std::cerr << "Error connecting to " << host << ": " << ssh_get_error(my_ssh_session) << std::endl;
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    // Authenticate
    password = getpass("Enter password: ");
    rc = ssh_userauth_password(my_ssh_session, nullptr, password);
    if (rc != SSH_AUTH_SUCCESS) {
        std::cerr << "Authentication failed: " << ssh_get_error(my_ssh_session) << std::endl;
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    // Create channel
    channel = ssh_channel_new(my_ssh_session);
    if (channel == nullptr) {
        std::cerr << "Error creating channel" << std::endl;
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    // Open channel
    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        std::cerr << "Error opening channel" << std::endl;
        ssh_channel_free(channel);
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    // Execute command
    rc = ssh_channel_request_exec(channel, command);
    if (rc != SSH_OK) {
        std::cerr << "Error executing command" << std::endl;
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        return EXIT_FAILURE;
    }

    // Read output
    char buffer[256];
    int nbytes;
    std::cout << "Command output:" << std::endl;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        std::cout.write(buffer, nbytes);
    }

    // Cleanup
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);

    return EXIT_SUCCESS;
}