#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define ECHOMAX 255     /* Longest string to echo */

void DieWithError(const char *errorMessage) /* External error handling function */
{
    perror(errorMessage);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sock;                        /* Socket */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int cliAddrLen;         /* Length of incoming message */
    char echoBuffer[ECHOMAX];        /* Buffer for echo string */
    unsigned short echoServPort;     /* Server port */
    int recvMsgSize;                 /* Size of received message */
    int numOfUser = 0;
    int numOfContactList = 0;
    char test[9][100];

    char contactList1[10][100];
    int NumList1 = 0;
    char contactList2[10][100];
    int NumList2 = 0;

    int imState = 0;
    char message[200];
    char back[100];
    for (int i = 0; i < 3; i++)
    {
        strncpy(contactList1[i * 3 + 1], " ", sizeof(" "));
        strncpy(contactList2[i * 3 + 1], " ", sizeof(" "));
    }
    strncpy(test[0], " ", sizeof(" "));
    strncpy(test[3], " ", sizeof(" "));
    strncpy(test[6], " ", sizeof(" "));

    if (argc != 2)         /* Test for correct number of parameters */
    {
        fprintf(stderr,"Usage:  %s <UDP SERVER PORT>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */

	//printf( "Before bind, IP address on the server is: %s\n", inet_ntoa(echoServAddr.sin_addr ) );
	
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

	//printf( "After bind, IP address on the server is: %s\n", inet_ntoa(echoServAddr.sin_addr ) );
	printf( "Port server is listening to is: %d\n", echoServPort );

    for (;;) /* Run forever */
    {
       
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(echoClntAddr);

        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
            DieWithError("recvfrom() failed");

		echoBuffer[ recvMsgSize ] = '\0';
        
        printf("\nServer handling client at IP address: %s\n", inet_ntoa(echoClntAddr.sin_addr));
        printf("Server receives string: %s\n", echoBuffer);
        if (strcmp(echoBuffer, "register") == 0)        //Comes here if echo buffer is equal to register
        {
            if (numOfUser < 3)      //If the num of user is lower than 3, we can create a new user
            {

                cliAddrLen = sizeof(echoClntAddr);

                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                echoBuffer[recvMsgSize] = '\0';
                strncpy(test[numOfUser*3], echoBuffer, sizeof(echoBuffer));     //waits for name and inputs it

                cliAddrLen = sizeof(echoClntAddr);
                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                echoBuffer[recvMsgSize] = '\0';
                strncpy(test[numOfUser * 3+1], echoBuffer, sizeof(echoBuffer));     //waits for IP and inputs it

                cliAddrLen = sizeof(echoClntAddr);
                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                echoBuffer[recvMsgSize] = '\0';
                strncpy(test[numOfUser * 3+2], echoBuffer, sizeof(echoBuffer));     //waits for Ports and inputs it

                numOfUser = numOfUser + 1;      //Add one to user
                char workinge[] = "Success";    
                if (sendto(sock, workinge, strlen(workinge), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(workinge))
                    DieWithError("sendto() sent a different number of bytes than expected");

            }
            else                             //Will wait to consume 3 input from clients if the number of user is bigger than 3
            {
                cliAddrLen = sizeof(echoClntAddr);

                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                cliAddrLen = sizeof(echoClntAddr);      //Wait and not use message

                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                cliAddrLen = sizeof(echoClntAddr);      //Wait and not use message

                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");      //Wait and not use message
                char returnFail[] = "FAILURE";      //Return failure because there was too many users
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
        }
        else if (strcmp(echoBuffer, "create") == 0)         //Check echo buffer and come here
        {
            if (numOfContactList < 2)           //Make sure the contact list is smaller than 2
            {
                cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                echoBuffer[recvMsgSize] = '\0';
                if (numOfContactList < 1)       //If Contact list is 0, set the name to the first list
                {
                    strncpy(contactList1[0], echoBuffer, sizeof(echoBuffer));
                }
                else                //Else add the name to the second list
                {
                    strncpy(contactList2[0], echoBuffer, sizeof(echoBuffer));
                }
                numOfContactList = numOfContactList + 1;        //Add to the total of list
                char workinge[] = "Success";
                if (sendto(sock, workinge, strlen(workinge), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(workinge))
                    DieWithError("sendto() sent a different number of bytes than expected");
                
            }
            else
            {           //Waits for the name because one is expect even if we do not use it
                cliAddrLen = sizeof(echoClntAddr);
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                char returnFail[] = "FAILURE";      //return failure
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
        }   
        else if (strcmp(echoBuffer, "query-lists") == 0)        //Come here to get the list.
        {
            if (numOfContactList == 0)      //Checks the number of contact list
            {
                char returnNum[] = "0";     //Return 0 to client
                if (sendto(sock, returnNum, strlen(returnNum), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnNum))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
            else if (numOfContactList == 1)  //Checks the number of contact list
            {
                char returnNum[] = "1"; //Return 1 to client and the name of the first list
                if (sendto(sock, returnNum, strlen(returnNum), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnNum))
                    DieWithError("sendto() sent a different number of bytes than expected");
                strncpy(back, contactList1[0], sizeof(contactList1[0]));
                if (sendto(sock, back, strlen(back), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(back))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
            else 
            {       //Return 2 to client and the name of the first list and the second list
                char returnNum[] = "2";
                if (sendto(sock, returnNum, strlen(returnNum), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnNum))
                    DieWithError("sendto() sent a different number of bytes than expected");
                strncpy(back, contactList1[0], sizeof(contactList1[0]));
                if (sendto(sock, back, strlen(back), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(back))
                    DieWithError("sendto() sent a different number of bytes than expected");
                strncpy(back, contactList2[0], sizeof(contactList2[0]));
                if (sendto(sock, back, strlen(back), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(back))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
                       
        }
        else if (strcmp(echoBuffer, "join") == 0)      //Come here if echobuffer is join
        {

            cliAddrLen = sizeof(echoClntAddr);

            /* Block until receive message from a client */
            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)   //await name of contact list
                DieWithError("recvfrom() failed");
            echoBuffer[recvMsgSize] = '\0';

            if (strcmp(echoBuffer, contactList1[0]) == 0 && NumList1 < 3)     //Check if the name of the list is correct
            {
                cliAddrLen = sizeof(echoClntAddr);

                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)       //waits for a name to add
                    DieWithError("recvfrom() failed");
                echoBuffer[recvMsgSize] = '\0';
                if (strcmp(echoBuffer, test[0]) == 0)       //Check the first user name adds it to the list if correct
                {
                    strncpy(contactList1[NumList1*3 + 1], test[0], sizeof(test[0]));
                    strncpy(contactList1[NumList1*3 + 2], test[1], sizeof(test[1]));
                    strncpy(contactList1[NumList1*3 + 3], test[2], sizeof(test[2]));
                    NumList1 = NumList1 + 1;
                    char returnFail[] = "SUCCESS";
                    if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                        DieWithError("sendto() sent a different number of bytes than expected");

                }
                else if (strcmp(echoBuffer, test[3]) == 0)          //Check the second user name and adds it to the list if correct
                {
                    strncpy(contactList1[NumList1*3 + 1], test[3], sizeof(test[3]));
                    strncpy(contactList1[NumList1*3 + 2], test[4], sizeof(test[4]));
                    strncpy(contactList1[NumList1*3 + 3], test[5], sizeof(test[5]));
                    NumList1 = NumList1 + 1;

                    char returnFail[] = "SUCCESS";
                    if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                        DieWithError("sendto() sent a different number of bytes than expected");
                }
                else if (strcmp(echoBuffer, test[6]) == 0)      //Check the third user name and adds it to the list if correct
                {
                    strncpy(contactList1[NumList1*3 + 1], test[6], sizeof(test[6]));
                    strncpy(contactList1[NumList1*3 + 2], test[7], sizeof(test[7]));
                    strncpy(contactList1[NumList1*3 + 3], test[8], sizeof(test[8]));
                    NumList1 = NumList1 + 1;
                    char returnFail[] = "SUCCESS";
                    if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                        DieWithError("sendto() sent a different number of bytes than expected");

                }
                else
                {       //return if failed 
                    char returnFail[] = "FAILURE";
                    if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                        DieWithError("sendto() sent a different number of bytes than expected");
                }
            }
            else if (strcmp(echoBuffer, contactList2[0]) == 0 && NumList2 < 3) //checks the name of the second list
            {
                cliAddrLen = sizeof(echoClntAddr);

                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0) //waits for a name
                    DieWithError("recvfrom() failed");
                echoBuffer[recvMsgSize] = '\0';

                if (strcmp(echoBuffer, test[0]) == 0)   //Check the first user name and adds it to the list if correct
                {
                    strncpy(contactList2[NumList2*3 + 1], test[0], sizeof(test[0]));
                    strncpy(contactList2[NumList2*3 + 2], test[1], sizeof(test[1]));
                    strncpy(contactList2[NumList2*3 + 3], test[2], sizeof(test[2]));
                    NumList2 = NumList2 + 1;
                    char returnFail[] = "SUCCESS";
                    if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                        DieWithError("sendto() sent a different number of bytes than expected");

                }
                else if (strcmp(echoBuffer, test[3]) == 0)
                {
                    strncpy(contactList2[NumList2*3 + 1], test[3], sizeof(test[3]));     //Check the second user name and adds it to the list if correct
                    strncpy(contactList2[NumList2*3 + 2], test[4], sizeof(test[4]));
                    strncpy(contactList2[NumList2*3 + 3], test[5], sizeof(test[5]));
                    NumList2 = NumList2 + 1;
                    char returnFail[] = "SUCCESS";
                    if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                        DieWithError("sendto() sent a different number of bytes than expected");
                }
                else if (strcmp(echoBuffer, test[6]) == 0)
                {
                    strncpy(contactList2[NumList2*3 + 1], test[6], sizeof(test[6]));     //Check the third user name and adds it to the list if correct
                    strncpy(contactList2[NumList2*3 + 2], test[7], sizeof(test[7]));
                    strncpy(contactList2[NumList2*3 + 3], test[8], sizeof(test[8]));
                    NumList2 = NumList2 + 1;
                    char returnFail[] = "SUCCESS";
                    if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                        DieWithError("sendto() sent a different number of bytes than expected");
                }
                else
                {   //Return failure if not found
                    char returnFail[] = "FAILURE";
                    if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                        DieWithError("sendto() sent a different number of bytes than expected");
                }
            }
            else
            {

                cliAddrLen = sizeof(echoClntAddr);

                /* Block until receive message from a client */
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                echoBuffer[recvMsgSize] = '\0';

                char returnFail[] = "FAILURE";
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");

            }
        }
        else if (strcmp(echoBuffer, "leave") == 0)  //Waits for two other messages
        {
        cliAddrLen = sizeof(echoClntAddr);

        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0) // Receive contact list name
            DieWithError("recvfrom() failed");
        echoBuffer[recvMsgSize] = '\0';

        if (strcmp(echoBuffer, contactList1[0]) == 0 && NumList1 >0) //Check if the first list has the same name
        {
            cliAddrLen = sizeof(echoClntAddr);

            /* Block until receive message from a client */
            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)  //Receive Name 
                DieWithError("recvfrom() failed");
            echoBuffer[recvMsgSize] = '\0';
            if (strcmp(echoBuffer, contactList1[1]) == 0)       //Checks for name in first spot
            {
                strncpy(contactList1[1], "", sizeof(""));
                strncpy(contactList1[2], "", sizeof(""));
                strncpy(contactList1[3], "", sizeof(""));
                NumList1 = NumList1 - 1;
                char returnFail[] = "SUCCESS";
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");

            }
            else if (strcmp(echoBuffer, contactList1[4]) == 0)      //Checks name for second spot
            {
                strncpy(contactList1[4], "", sizeof(""));
                strncpy(contactList1[5], "", sizeof(""));
                strncpy(contactList1[6], "", sizeof(""));
                NumList1 = NumList1 - 1;

                char returnFail[] = "SUCCESS";
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
            else if (strcmp(echoBuffer, contactList1[7]) == 0)      //Checks name for third spot
            {
                strncpy(contactList1[7], "", sizeof(""));
                strncpy(contactList1[8], "", sizeof(""));
                strncpy(contactList1[9], "", sizeof(""));
                NumList1 = NumList1 - 1;
                char returnFail[] = "SUCCESS";
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");

            }
            else
            {                                           //If not found return failuer
                char returnFail[] = "FAILURE";
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
        }
        else if (strcmp(echoBuffer, contactList2[0]) == 0 && NumList2 < 3)  //Check if contact name list  is contact list2
        {
            cliAddrLen = sizeof(echoClntAddr);

            /* Block until receive message from a client */
            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0) //waits for name
                DieWithError("recvfrom() failed");
            echoBuffer[recvMsgSize] = '\0';

            if (strcmp(echoBuffer, contactList2[1]) == 0) //Checks for name in first spot
            {
                strncpy(contactList2[1], "", sizeof(""));
                strncpy(contactList2[2], "", sizeof(""));
                strncpy(contactList2[3], "", sizeof(""));
                NumList2 = NumList2 - 1;
                char returnFail[] = "SUCCESS";
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");

            }
            else if (strcmp(echoBuffer, contactList2[4]) == 0) //Checks for name in second spot
            {
                strncpy(contactList2[4], "", sizeof(""));
                strncpy(contactList2[5], "", sizeof(""));
                strncpy(contactList2[6], "", sizeof(""));
                NumList2 = NumList2 - 1;
                char returnFail[] = "SUCCESS";
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
            else if (strcmp(echoBuffer, contactList2[7]) == 0)
            {
                strncpy(contactList2[7], "", sizeof(""));   //Checks for name in third spot
                strncpy(contactList2[8], "", sizeof(""));
                strncpy(contactList2[9], "", sizeof(""));
                NumList2 = NumList2 - 1;
                char returnFail[] = "SUCCESS";
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
            else
            {
                char returnFail[] = "FAILURE";      //return failure if the name was not found
                if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                    DieWithError("sendto() sent a different number of bytes than expected");
            }
        }
        else
        {

            cliAddrLen = sizeof(echoClntAddr);

            /* Block until receive message from a client */
            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                DieWithError("recvfrom() failed");
            echoBuffer[recvMsgSize] = '\0';

            char returnFail[] = "FAILURE";
            if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                DieWithError("sendto() sent a different number of bytes than expected");

        }
        }
        else if (strcmp(echoBuffer, "save") == 0) //Check if save is receive and goes in here
        {

            cliAddrLen = sizeof(echoClntAddr);

            /* Block until receive message from a client */
            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)  //waits for the name of the files
                DieWithError("recvfrom() failed");
            echoBuffer[recvMsgSize] = '\0';

            char name[50];
           // strncpy(name, echoBuffer);      //Trying to add .txt but it isnt working at the moment
            //strcat(name, ".txt");
            FILE *saved;
            saved = fopen("test.txt", "w");
            fprintf(saved, "Active Users: ");       //Print The number of active users
            fprintf(saved, "%d",numOfUser);
                fprintf(saved,  "\n");
            if (strcmp(test[0], " ") != 0)          // Print first user if one is there
            {
                fprintf(saved, "Name: ");
                fprintf(saved, test[0]);
                fprintf(saved, " IP: ");
                fprintf(saved, test[1]);
                fprintf(saved, " Port: ");
                fprintf(saved, test[2]);
                fprintf(saved, "\n");
            
            }
            if (strcmp(test[3], " ") != 0 && numOfUser > 1) // Print Second user if one is there
            {
                fprintf(saved, "Name: ");
                fprintf(saved, test[3]);
                fprintf(saved, " IP: ");
                fprintf(saved, test[4]);
                fprintf(saved, " Port: ");
                fprintf(saved, test[5]);
                fprintf(saved, "\n");
            }
            if (strcmp(test[6], " ") != 0 && numOfUser > 2) // Print third user if one is there
            {
                fprintf(saved, "Name: ");
                fprintf(saved, test[6]);
                fprintf(saved, " IP: ");
                fprintf(saved, test[7]);
                fprintf(saved, " Port: ");
                fprintf(saved, test[8]);
                fprintf(saved, "\n");
            }
            fprintf(saved, "Number of contact lists: "); //Print the number of contact list
            fprintf(saved, "%d",numOfContactList);
            fprintf(saved, "\n");

            if (numOfContactList >= 1)
            {
                fprintf(saved, "Contact list name: ");      //Print name of contact list
                fprintf(saved, contactList1[0]);
                fprintf(saved, " Number of contact in list: "); //Print the number of contact in the list
                fprintf(saved, "%d", NumList1);
                fprintf(saved, "\n");

                if (strcmp(contactList1[1], " " ) != 0)     //Print first user if one is there
                {
                    fprintf(saved, "Name: ");
                    fprintf(saved, contactList1[1]);
                    fprintf(saved, " IP: ");
                    fprintf(saved, contactList1[2]);
                    fprintf(saved, " Port: ");
                    fprintf(saved, contactList1[3]);
                    fprintf(saved, "\n");

                }
                if (strcmp(contactList1[4], " ") != 0)      //Print second user if one is there
                {
                    fprintf(saved, "Name: ");
                    fprintf(saved, contactList1[4]);
                    fprintf(saved, " IP: ");
                    fprintf(saved, contactList1[5]);
                    fprintf(saved, " Port: ");
                    fprintf(saved, contactList1[6]);
                    fprintf(saved, "\n");

                }
                if (strcmp(contactList1[7], " ") != 0)      //Print third user if one is there
                {
                    fprintf(saved, "Name: ");
                    fprintf(saved, contactList1[7]);
                    fprintf(saved, " IP: ");
                    fprintf(saved, contactList1[8]);
                    fprintf(saved, " Port: ");
                    fprintf(saved, contactList1[9]);
                    fprintf(saved, "\n");

                }
            }
            if (numOfContactList == 2)                  
            {
                fprintf(saved, "Contact list name: ");              //Print name of contact list
                fprintf(saved,contactList2[0]);
                fprintf(saved, " Number of contact in list: ");     //Print the number of contact in the list
                fprintf(saved, "%d", NumList2);
                fprintf(saved, "\n");

                if (strcmp(contactList2[1], " " ) != 0)     //Print first user if one is there
                {
                    fprintf(saved, "Name: ");
                    fprintf(saved, contactList2[1]);
                    fprintf(saved, " IP: ");
                    fprintf(saved, contactList2[2]);
                    fprintf(saved, " Port: ");
                    fprintf(saved, contactList2[3]);
                    fprintf(saved, "\n");

                }
                if (strcmp(contactList2[4], " " ) != 0)     //Print second user if one is there
                {  
                    fprintf(saved, "Name: ");
                    fprintf(saved, contactList2[4]);
                    fprintf(saved, " IP: ");
                    fprintf(saved, contactList2[5]);
                    fprintf(saved, " Port: ");
                    fprintf(saved, contactList2[6]);
                    fprintf(saved, "\n");

                }
                if (strcmp(contactList2[7], " ") != 0)      //Print third user if one is there
                {
                    fprintf(saved, "Name: ");
                    fprintf(saved, contactList2[7]);
                    fprintf(saved, " IP: ");
                    fprintf(saved, contactList2[8]);
                    fprintf(saved, " Port: ");
                    fprintf(saved, contactList2[9]);
                    fprintf(saved, "\n");

                }
            }
            fclose(saved);          //Close the file
            char returnFail[] = "SUCCESS";  //Return Succes
            if (sendto(sock, returnFail, strlen(returnFail), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(returnFail))
                DieWithError("sendto() sent a different number of bytes than expected");


        }
        else if (strcmp(echoBuffer, "exit") == 0)  //Listen for the message but does not act on it as of now
        {

            cliAddrLen = sizeof(echoClntAddr);

            /* Block until receive message from a client */
            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                DieWithError("recvfrom() failed");
            echoBuffer[recvMsgSize] = '\0';
        }
        else if (strcmp(echoBuffer, "im-start") == 0)
        {
            imState = 1;
            cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                DieWithError("recvfrom() failed");
            echoBuffer[recvMsgSize] = '\0';
            if (strcmp(echoBuffer, contactList1[0]) == 0)
            {
                cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                    DieWithError("recvfrom() failed");
                echoBuffer[recvMsgSize] = '\0';
                if (strcmp(echoBuffer, contactList1[1]) == 0)
                {
                    cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                    if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                        DieWithError("recvfrom() failed");
                    echoBuffer[recvMsgSize] = '\0';
                    strncpy(message, echoBuffer, sizeof(echoBuffer));
                    
                    char workinge[] = "Message from: ";
                    if (sendto(sock, workinge, strlen(workinge), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(workinge))
                        DieWithError("sendto() sent a different number of bytes than expected");
                    int sender = 1;
                    if (NumList1 == 2)
                    {
                        sender = 4;
                    }
                    else if (NumList1 == 3)
                    {
                        sender = 7;
                    }

                    strncpy(back, contactList1[sender], sizeof(contactList1[sender]));
                    if (sendto(sock, back, strlen(back), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(back))
                        DieWithError("sendto() sent a different number of bytes than expected");

                    if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(message))
                        DieWithError("sendto() sent a different number of bytes than expected");


                    while (imState == 1)
                    {
                        cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                            DieWithError("recvfrom() failed");
                        echoBuffer[recvMsgSize] = '\0';
                        if (strcmp(echoBuffer, "im-complete") == 0)
                        {
                            cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                                DieWithError("recvfrom() failed");
                            echoBuffer[recvMsgSize] = '\0';
                            if (strcmp(echoBuffer, contactList1[0]) == 0)
                            {
                                cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                                    DieWithError("recvfrom() failed");
                                echoBuffer[recvMsgSize] = '\0';
                                if (strcmp(echoBuffer, contactList1[1]) == 0)
                                {
                                    imState = 0;
                                    char workinge[] = "Success";
                                    if (sendto(sock, workinge, strlen(workinge), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(workinge))
                                        DieWithError("sendto() sent a different number of bytes than expected");
                                }
                            }
                        }
                    }

                }
                else if (strcmp(echoBuffer, contactList2[0]) == 0)
                {
                    cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                    if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                        DieWithError("recvfrom() failed");
                    echoBuffer[recvMsgSize] = '\0';
                    if (strcmp(echoBuffer, contactList2[1]) == 0)
                    {
                        cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                            DieWithError("recvfrom() failed");
                        echoBuffer[recvMsgSize] = '\0';
                        strncpy(message, echoBuffer, sizeof(echoBuffer));

                        char workinge[] = "Message from: ";
                        if (sendto(sock, workinge, strlen(workinge), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(workinge))
                            DieWithError("sendto() sent a different number of bytes than expected");
                        int sender = 1;
                        if (NumList2 == 2)
                        {
                            sender = 4;
                        }
                        else if (NumList2 == 3)
                        {
                            sender = 7;
                        }

                        strncpy(back, contactList2[sender], sizeof(contactList2[sender]));
                        if (sendto(sock, back, strlen(back), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(back))
                            DieWithError("sendto() sent a different number of bytes than expected");

                        if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(message))
                            DieWithError("sendto() sent a different number of bytes than expected");


                        while (imState == 1)
                        {
                            cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                            if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                                DieWithError("recvfrom() failed");
                            echoBuffer[recvMsgSize] = '\0';
                            if (strcmp(echoBuffer, "im-complete") == 0)
                            {
                                cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                                if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                                    DieWithError("recvfrom() failed");
                                echoBuffer[recvMsgSize] = '\0';
                                if (strcmp(echoBuffer, contactList2[0]) == 0)
                                {
                                    cliAddrLen = sizeof(echoClntAddr);      //Waits for the name
                                    if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&echoClntAddr, &cliAddrLen)) < 0)
                                        DieWithError("recvfrom() failed");
                                    echoBuffer[recvMsgSize] = '\0';
                                    if (strcmp(echoBuffer, contactList2[1]) == 0)
                                    {
                                        imState = 0;
                                        char workinge[] = "Success";
                                        if (sendto(sock, workinge, strlen(workinge), 0, (struct sockaddr*)&echoClntAddr, sizeof(echoClntAddr)) != strlen(workinge))
                                            DieWithError("sendto() sent a different number of bytes than expected");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /*printf("\nServer handling client at IP address: %s\n", inet_ntoa( echoClntAddr.sin_addr ));
        printf("Server receives string: %s\n", echoBuffer );
		printf("Server sending same string back to client: %s\n", echoBuffer );

        /* Send received datagram back to the client */
       /* if (sendto(sock, echoBuffer, strlen(echoBuffer), 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != strlen(echoBuffer))
            DieWithError("sendto() sent a different number of bytes than expected");*/
    }
    /* NOT REACHED */
}
