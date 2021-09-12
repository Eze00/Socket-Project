#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define ECHOMAX 255 /* Longest string to echo */
#define ITERATIONS 5

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[])
{
    int n;
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short echoServPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char *servIP;                    /* IP address of server */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[ECHOMAX + 1];    /* Buffer for receiving echoed string */
    int echoStringLen;               /* Length of string to echo */
    int respStringLen;               /* Length of received response */

    if (argc < 3) /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server IP address> <Echo Port>\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Second arg: Use given port, if any */

    printf("Arguments passed: server IP %s, port %d\n", servIP, echoServPort);

    /* Create a datagram/UDP socket */

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */

    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
    echoServAddr.sin_port = htons(echoServPort);      /* Server port */

    /* Pass string back and forth between server 5 times */

    printf("Echoing %d iterations.\n", ITERATIONS);

    while (strcmp(echoBuffer, "exit") != 0)
    {
        // printf("\nEnter string to echo: \n");
        // scanf("%s", echoBuffer);
        // printf("\nClient sends string ``%s'' to server\n", echoBuffer);

        // /* Send the struct to the server */
        // if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))
        //     DieWithError("sendto() sent a different number of bytes than expected");

        // /* Receive a response */

        // fromSize = sizeof(fromAddr);

        // if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *)&fromAddr, &fromSize)) > ECHOMAX)
        //     DieWithError("recvfrom() failed");

        // echoBuffer[respStringLen] = '\0';

        // if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
        // {
        //     fprintf(stderr, "Error: received a packet from unknown source.\n");
        //     exit(1);
        // }

        // printf("Client received string from server: ``%s''\n", echoBuffer); /* Print the echoed arg */
    }

    close(sock);
    exit(0);
}
