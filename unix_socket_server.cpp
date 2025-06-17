#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>


int main(){
    int sockfd, client_fd;
    
    /*
     struct sockaddr_un{
        sa_family_t sun_family    // Address family
        char        sun_path[108] // Path to the socket file
     }
     */
    
    struct sockaddr_un addr;
    char buffer[100];
    
    // Create A Unix domain socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(sockfd == -1){
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }
    
    // Setting all values in addr to zero
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    
    // Copies string "/tmp/my_unix_socket" to the addr.sun_path field
    strcpy(addr.sun_path, "/tmp/my_unix_socket");
    
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

    
    
}
    

