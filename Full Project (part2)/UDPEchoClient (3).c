#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define ECHOMAX 255     /* Longest string to echo */
#define ITERATIONS	500

void DieWithError(const char* errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char* argv[])
{
    int n;
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;     /* Source address of echo */
    unsigned short echoServPort;     /* Echo server port */
    unsigned int fromSize;           /* In-out of address size for recvfrom() */
    char* servIP;                    /* IP address of server */
    char* echoString;                /* String to send to echo server */
    char echoBuffer[ECHOMAX + 1];      /* Buffer for receiving echoed string */
    int echoStringLen;               /* Length of string to echo */
    int respStringLen;               /* Length of received response */

    if (argc < 3)    /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server IP address> <Echo Port>\n", argv[0]);
        exit(1);
    }

    servIP = argv[1];           /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]);  /* Second arg: Use given port, if any */

    printf("Arguments passed: server IP %s, port %d\n", servIP, echoServPort);

    /* Create a datagram/UDP socket */

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */

    memset(&echoServAddr, 0, sizeof(echoServAddr));    /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                 /* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);  /* Server IP address */
    echoServAddr.sin_port = htons(echoServPort);     /* Server port */

    /* Pass string back and forth between server 5 times */

    //printf("Echoing %d iterations.\n", ITERATIONS);

    for (int i = 0; i < ITERATIONS; i++)
    {
        printf("\nEnter string to echo: \n");
        scanf("%s\n", echoBuffer);
        //printf( "\nClient sends string ``%s'' to server\n", echoBuffer );	

        /* Send the struct to the server */
        if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))
            DieWithError("sendto() sent a different number of bytes than expected");

        if (strcmp(echoBuffer, "register") == 0)  //Check if the messages transmitted to the servers, and transmitte 3 more and awaits a response. 
        {
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))       //Next 3 messages
                DieWithError("sendto() sent a different number of bytes than expected");
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))
                DieWithError("sendto() sent a different number of bytes than expected");
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))
                DieWithError("sendto() sent a different number of bytes than expected");
            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer);     //Print either Success or Failure
        }
        else if (strcmp(echoBuffer, "create") == 0)     //Check if the messages transmited is create, gives another message to the server and wait for a response.
        {
            scanf("%s", echoBuffer); 
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))   //Second message
                DieWithError("sendto() sent a different number of bytes than expected");    
            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer);         //Print success or failure
        }
        else if (strcmp(echoBuffer, "query-lists") == 0)    //Semd a message for the lists contact
        {
            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)     //waits for a reply
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer);    /* Print the echoed arg */           //Prints the int that is return
            
            if (strcmp(echoBuffer, "1") == 0)       //If the return int is 1 waits for one more message from the server
            {
                fromSize = sizeof(fromAddr);

                if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                    DieWithError("recvfrom() failed");

                echoBuffer[respStringLen] = '\0';

                if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
                {
                    fprintf(stderr, "Error: received a packet from unknown source.\n");
                    exit(1);
                }

                printf("%s\n", echoBuffer);    /* Print the echoed arg */ //Print the name of the first contact list
            }
            else if(strcmp(echoBuffer, "2") == 0) //If the return number is 2 waits for two more messages from the server
            {
                fromSize = sizeof(fromAddr);

                if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                    DieWithError("recvfrom() failed");

                echoBuffer[respStringLen] = '\0';

                if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
                {
                    fprintf(stderr, "Error: received a packet from unknown source.\n");
                    exit(1);
                }

                printf("%s\n", echoBuffer);    /* Print the echoed arg */  //Print the nane of the first contact list
                fromSize = sizeof(fromAddr);

                if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                    DieWithError("recvfrom() failed");

                echoBuffer[respStringLen] = '\0';

                if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
                {
                    fprintf(stderr, "Error: received a packet from unknown source.\n");
                    exit(1);
                }

                printf("%s\n", echoBuffer);    /* Print the echoed arg */ //Print the nane of the second contact list
            }
        }

        else if (strcmp(echoBuffer, "join") == 0)       //Wait here to sent 2 more messages with the contact list and the contact that will join
        {
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer)) //Send both over
                DieWithError("sendto() sent a different number of bytes than expected");
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))
                DieWithError("sendto() sent a different number of bytes than expected");

            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)   //Waits for a reply from the server
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer);    /* Print the echoed arg */  //Print Success or Fail.
        }
        else if (strcmp(echoBuffer, "leave") == 0)      //waits here to send 2 more messages over with contact list name and the name
        {
            
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer)) // Send both over
                DieWithError("sendto() sent a different number of bytes than expected");
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))
                DieWithError("sendto() sent a different number of bytes than expected");

            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)     //Waits for a response
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer); // Print Success or Failure
        }
        else if (strcmp(echoBuffer, "save") == 0) //Wait to send another  message and awaits a message from server
        {
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer)) // Send message
                DieWithError("sendto() sent a different number of bytes than expected");


            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)  //waits for the server
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer); //Print Success or Failure

        }
        else if (strcmp(echoBuffer, "exit") == 0)       //For now, exit doesnt take away people from contact list and the users. But it exits the program for us
        {
            scanf("%s", echoBuffer);            
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))       //Still send a message over but for now doesnt do anything
                DieWithError("sendto() sent a different number of bytes than expected");
            i = 500; // i to 500 to exit
        }
        else if (strcmp(echoBuffer, "im-start") == 0)
        {
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer)) //Send both over
                DieWithError("sendto() sent a different number of bytes than expected");

            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))       //Still send a message over but for now doesnt do anything
                DieWithError("sendto() sent a different number of bytes than expected");

            scanf("%[^\n]%*c", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer))       //Still send a message over but for now doesnt do anything
                DieWithError("sendto() sent a different number of bytes than expected");

            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer);    /* Print the echoed arg */ //Print the nane of the second contact list
            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer);    /* Print the echoed arg */ //Print the nane of the second contact list
            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer);    /* Print the echoed arg */ //Print the nane of the second contact list
            
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer)) //Send both over
                DieWithError("sendto() sent a different number of bytes than expected");
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer)) //Send both over
                DieWithError("sendto() sent a different number of bytes than expected");
            scanf("%s", echoBuffer);
            if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != strlen(echoBuffer)) //Send both over
                DieWithError("sendto() sent a different number of bytes than expected");
            fromSize = sizeof(fromAddr);

            if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize)) > ECHOMAX)
                DieWithError("recvfrom() failed");

            echoBuffer[respStringLen] = '\0';

            if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
            {
                fprintf(stderr, "Error: received a packet from unknown source.\n");
                exit(1);
            }

            printf("%s\n", echoBuffer);    /* Print the echoed arg */ //Print the nane of the second contact list

        }

    	/* Receive a response */

   		/*fromSize = sizeof(fromAddr);

   		if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &fromAddr, &fromSize)) > ECHOMAX )
       		DieWithError("recvfrom() failed");

		echoBuffer[ respStringLen] = '\0';

   		if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
   		{
       			fprintf(stderr,"Error: received a packet from unknown source.\n");
       			exit(1);
   		}

 		printf("``%s''\n", echoBuffer );    /* Print the echoed arg */ 
 
	}
    
    close(sock);
    exit(0);
}
