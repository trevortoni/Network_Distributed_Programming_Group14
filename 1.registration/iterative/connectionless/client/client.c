#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 20000

int main()
{
    int clientSocket, ret;
    struct sockaddr_in cliAddr;
    socklen_t addrLen = sizeof(cliAddr);
    char buffer[1024];
    char response[1024];

    // Create the client socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);  // Use SOCK_DGRAM for UDP
    if (clientSocket < 0) {
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Client Socket is created.\n");

    memset(&cliAddr, '\0', sizeof(cliAddr));
    memset(buffer, '\0', sizeof(buffer));

    // Configure the client address
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_port = htons(PORT);
    cliAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char serialNumber[20];
    char regNumber[20];
    char name[100];

    printf("Enter Serial Number: ");
    scanf("%s", serialNumber);
    printf("Enter Registration Number: ");
    scanf("%s", regNumber);
    printf("Enter Name: ");
    scanf(" %[^\n]", name);

    // Formulate the registration  request
    snprintf(buffer, sizeof(buffer), "Serial Number: %s, Registration Number: %s, Name: %s", serialNumber, regNumber, name);

    // Send the registration request to the server
    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr*)&cliAddr, sizeof(cliAddr));

    // Receive the server response
    if (recvfrom(clientSocket, response, 1024, 0, (struct sockaddr*)&cliAddr, &addrLen) < 0) {
        printf("Error in receiving data.\n");
    } else {
        printf("Server: %s\n", response);
    }

    // Close the client socket
    close(clientSocket);
    return 0;
}
