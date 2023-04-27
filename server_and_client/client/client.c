#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_NAME_LENGTH 50

struct Record
{
    int sequence_num;
    char admission_num[15];
    char name[MAX_NAME_LENGTH];
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }
    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
// create socket
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error\n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported\n");
        return 1;
    }

// connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection failed\n");
        return 1;
    }

// prompt user for record details
// space before the format specifier '%[^\n]' is necessary
    struct Record new_record;
    printf("Enter sequence number: ");
    scanf("%d", &new_record.sequence_num);
    printf("Enter admission number: ");
    scanf(" %[^\n]", new_record.admission_num);
    printf("Enter your name: ");
    scanf(" %[^\n]", new_record.name);


// send record to server
    write(sock, &new_record, sizeof(new_record));

// wait for response
    int response;
    read(sock, &response, sizeof(response));
    if (response == 1)
    {
        printf("Duplicate record found! Record not saved.\n");
    }
    else
    {
        printf("Record saved successfully.\n");
    }

    close(sock);
    return 0;
}
