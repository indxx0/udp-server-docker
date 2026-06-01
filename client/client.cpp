#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);

    struct hostent *host = gethostbyname("udp-server");
    
    if (host == nullptr) {
        std::cerr << "Cannot resolve hostname 'udp-server'" << std::endl;
        return 1;
    }

    memcpy(&servaddr.sin_addr, host->h_addr, host->h_length);

    char buffer[1024];
    while (true) {
        std::string message = "Ping";
        sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
        std::cout << "Sent: " << message << std::endl;

        struct sockaddr_in from;
        socklen_t len = sizeof(from);
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, &len);
        
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Received echo: " << buffer << std::endl;
        }
        
        sleep(5);
    }
    close(sockfd);
    return 0;
}