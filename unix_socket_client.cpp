#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

#define SOCKET_PATH "/tmp/mysocket"

int main() {
    int client_fd;
    struct sockaddr_un addr;

    // 1. Create socket
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        return 1;
    }

    // 2. Prepare address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // 3. Connect to server
    if (connect(client_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        close(client_fd);
        return 1;
    }

    // 4. Send message to server
    const char* msg = "Hello from client!";
    write(client_fd, msg, strlen(msg));

    // 5. Read response from server (optional)
    char buffer[128];
    ssize_t bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        std::cout << "Server says: " << buffer << std::endl;
    }

    // 6. Close socket
    close(client_fd);
    return 0;
}

