#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "error with socket initialization" << std::endl;
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "error with bind" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "UDP echo server started on port " << PORT << "..." << std::endl;


    while (true) {
        socklen_t len = sizeof(cliaddr);
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, 0, 
                        (struct sockaddr *)&cliaddr, &len);
        
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Received: " << buffer << std::endl;

            sendto(sockfd, (const char *)buffer, n, 0, 
                  (const struct sockaddr *)&cliaddr, len);
        }
    }

    close(sockfd);
    return 0;
}