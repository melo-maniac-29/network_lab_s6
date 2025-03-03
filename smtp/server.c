
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> // For close()

#define MAXLINE 100

int main(int argc, char *argv[])
{
    int n;
    int sock_fd;
    struct sockaddr_in servaddr, cliaddr;
    char buf[MAXLINE + 1];
    socklen_t len;
    FILE *fp;

    // Check if the correct number of arguments is provided
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // Create a UDP socket
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Cannot create socket");
        exit(1);
    }

    // Initialize the server address structure
    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[1])); // Convert port number from string to network byte order
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any local interface

    // Bind the socket to the server address
    if (bind(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Bind failed");
        close(sock_fd);
        exit(1);
    }

    printf("SMTP server running on port %s...\n", argv[1]);
    
    len = sizeof(cliaddr);
    
    while (1)
    {
        // Receive initial SMTP request from client
        if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) == -1)
        {
            perror("recvfrom failed");
            continue;
        }
        buf[n] = '\0';
        printf("C:%s", buf);

        // Send response to client
        snprintf(buf, MAXLINE, "220 Simple Mail Transfer Service Ready\n");
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&cliaddr, len);

        // Process HELO command
        if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) == -1)
        {
            perror("recvfrom failed");
            continue;
        }
        buf[n] = '\0';
        printf("C:%s", buf);

        // Send HELO response
        snprintf(buf, MAXLINE, "250 Hello, pleased to meet you\n");
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&cliaddr, len);

        // Process MAIL FROM command
        if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) == -1)
        {
            perror("recvfrom failed");
            continue;
        }
        buf[n] = '\0';
        printf("C:%s", buf);

        // Send MAIL FROM response
        snprintf(buf, MAXLINE, "250 Sender OK\n");
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&cliaddr, len);

        // Process RCPT TO command
        if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) == -1)
        {
            perror("recvfrom failed");
            continue;
        }
        buf[n] = '\0';
        printf("C:%s", buf);

        // Send RCPT TO response
        snprintf(buf, MAXLINE, "250 Recipient OK\n");
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&cliaddr, len);

        // Process DATA command
        if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) == -1)
        {
            perror("recvfrom failed");
            continue;
        }
        buf[n] = '\0';
        printf("C:%s", buf);

        // Send DATA response
        snprintf(buf, MAXLINE, "354 Enter mail, end with \".\" on a line by itself\n");
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&cliaddr, len);

        // Create a file to store the email
        fp = fopen("email.txt", "w");
        if (fp == NULL)
        {
            perror("Cannot create file");
            continue;
        }

        // Receive email content
        while (1)
        {
            if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) == -1)
            {
                perror("recvfrom failed");
                fclose(fp);
                break;
            }
            buf[n] = '\0';
            printf("C:%s", buf);
            
            // Write content to file
            fprintf(fp, "%s", buf);
            
            // Check if message ended with a single "."
            if (strcmp(buf, ".\n") == 0 || strcmp(buf, ".") == 0)
            {
                fclose(fp);
                break;
            }
        }

        // Send message accepted response
        snprintf(buf, MAXLINE, "250 Message accepted for delivery\n");
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&cliaddr, len);

        // Process QUIT command
        if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len)) == -1)
        {
            perror("recvfrom failed");
            continue;
        }
        buf[n] = '\0';
        printf("C:%s", buf);

        // Send QUIT response
        snprintf(buf, MAXLINE, "221 Bye\n");
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&cliaddr, len);
        
        printf("Email transaction completed\n");
    }

    close(sock_fd); // Close the socket
    return 0;
}