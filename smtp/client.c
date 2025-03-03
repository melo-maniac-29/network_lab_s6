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
    struct sockaddr_in servaddr;
    char buf[MAXLINE + 1];
    char address_buf[MAXLINE], message_buf[MAXLINE];
    char *str;

    // Check if the correct number of arguments is provided
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    // Create a UDP socket
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Cannot create socket");
        exit(1);
    }

    // Initialize the server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2])); // Convert port number from string to network byte order
    
    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock_fd);
        exit(1);
    }

    // Send initial SMTP request
    snprintf(buf, MAXLINE, "SMTP REQUEST FROM CLIENT\n");
    n = sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (n < 0)
    {
        perror("sendto failed");
        close(sock_fd);
        exit(1);
    }

    // Receive server response
    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1)
    {
        perror("recvfrom failed");
        close(sock_fd);
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    // Send HELO command
    snprintf(buf, MAXLINE, "HELO client.example.com\n");
    n = sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (n < 0)
    {
        perror("sendto failed");
        close(sock_fd);
        exit(1);
    }

    // Receive server response
    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1)
    {
        perror("recvfrom failed");
        close(sock_fd);
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    // Get sender email address from user
    printf("Please enter the email address of the sender: ");
    if (fgets(address_buf, sizeof(address_buf), stdin) == NULL) {
        perror("Error reading input");
        close(sock_fd);
        exit(1);
    }
    address_buf[strcspn(address_buf, "\n")] = '\0'; // Remove newline character safely

    // Send MAIL FROM command
    snprintf(buf, MAXLINE, "MAIL FROM:<%s>\n", address_buf);
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive server response
    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1)
    {
        perror("recvfrom failed");
        close(sock_fd);
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    // Get receiver email address from user
    printf("Please enter the email address of the receiver: ");
    if (fgets(address_buf, sizeof(address_buf), stdin) == NULL) {
        perror("Error reading input");
        close(sock_fd);
        exit(1);
    }
    address_buf[strcspn(address_buf, "\n")] = '\0'; // Remove newline character safely

    // Send RCPT TO command
    snprintf(buf, MAXLINE, "RCPT TO:<%s>\n", address_buf);
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive server response
    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1)
    {
        perror("recvfrom failed");
        close(sock_fd);
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    // Send DATA command
    snprintf(buf, MAXLINE, "DATA\n");
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive server response
    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1)
    {
        perror("recvfrom failed");
        close(sock_fd);
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    // Send email message
    printf("Enter email message. End with a line containing only a period (.)\n");
    do
    {
        if (fgets(message_buf, sizeof(message_buf), stdin) == NULL) {
            perror("Error reading input");
            close(sock_fd);
            exit(1);
        }
        sendto(sock_fd, message_buf, strlen(message_buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        
        // Check if message ended with a single "."
        if (strcmp(message_buf, ".\n") == 0 || strcmp(message_buf, ".") == 0)
            break;
            
    } while (1);

    // Receive server response
    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1)
    {
        perror("recvfrom failed");
        close(sock_fd);
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    // Send QUIT command
    snprintf(buf, MAXLINE, "QUIT\n");
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive server response
    if ((n = recvfrom(sock_fd, buf, MAXLINE, 0, NULL, NULL)) == -1)
    {
        perror("recvfrom failed");
        close(sock_fd);
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    printf("Email sent successfully\n");
    
    close(sock_fd); // Close the socket
    return 0;
}