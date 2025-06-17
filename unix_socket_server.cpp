#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#define SOCKET_PATH "/tmp/mysocket"


int main(){
    int sockfd, client_fd;
    
    /*
     struct sockaddr_un{
        sa_family_t sun_family    // Address family -> AF_UNIX declared below
        char        sun_path[108] // Path to the socket file
     }
     */
    
    struct sockaddr_un addr;
    char buffer[100];
    
    // Create A Unix domain socket
    // Returns a file descriptor
    // Domain, Connection Oriented, Default Protocol
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(sockfd == -1){
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }
    
    // Setting all values in struct addr to zero
    memset(&addr, 0, sizeof(struct sockaddr_un));
    
    // sun_family type to AF_UNIX
    addr.sun_family = AF_UNIX;
    
    // Copies string "/tmp/my_unix_socket" to the addr.sun_path field
    strcpy(addr.sun_path, SOCKET_PATH);
    
    // Binding socket
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        std::cerr << "Bind failed!" << std::endl;
        close(sockfd);
        return 1;
    }
    
    // Listen for incoming connections
    if (listen(sockfd, 1) == -1) {
        std::cerr << "Listen failed!" << std::endl;
        close(sockfd);
        return 1;
    }
    
    std::cout << "Server listening on " << SOCKET_PATH << std::endl;
    
    client_fd = accept(sockfd, NULL, NULL);
    if (client_fd == -1) {
        std::cerr << "Accept failed!" << std::endl;
        close(sockfd);
        return 1;
    }
    
    // Read data from the client
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        std::cerr << "Read failed!" << std::endl;
        close(client_fd);
        close(sockfd);
        return 1;
    }

    buffer[bytes_read] = '\0';  // Null terminate the string
    std::cout << "Received message: " << buffer << std::endl;

    // Send a response to the client
    const char* response = "Message received";
    if (write(client_fd, response, strlen(response)) == -1) {
        std::cerr << "Write failed!" << std::endl;
        close(client_fd);
        close(sockfd);
        return 1;
    }

    // Clean up
    close(client_fd);
    close(sockfd);
    unlink(SOCKET_PATH);
    
}
    

