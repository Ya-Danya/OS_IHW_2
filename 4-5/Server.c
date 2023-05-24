#include "header.h"

#define MAXPENDING 3    /* Maximum outstanding connection requests */

void bubbleSort(Catalog *lib) {
    Book *arr = lib->books;
    int n = lib->num_books;
    int i, j;
    Book temp;
    
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j].id > arr[j+1].id) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void DieWithError(char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket);   /* TCP client handling function */

int main(int argc, char *argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */
    Catalog library;
    int amount_of_books = atoi(argv[2]);
    library.books = (Book*) malloc(amount_of_books);

    if (argc != 2)     /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);  /* First arg:  local port */

    /* Create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

    printf("Server IP address = %s. Wait...\n", inet_ntoa(echoClntAddr.sin_addr));

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    while (amount_of_books != library.num_books)/* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            DieWithError("accept() failed");

        /* clntSock is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        Book bookBuffer;                   /* Buffer for echo string */
        int recvMsgSize;                    /* Size of received message */

        /* Receive message from client */
        if ((recvMsgSize = recv(clntSock, &bookBuffer, sizeof(Book), 0)) < 0)
        DieWithError("recv() failed");

        /* Send received string and receive again until end of transmission */
        while (recvMsgSize > 0)      /* zero indicates end of transmission */
        {
            

            /* See if there is more data to receive */
            if ((recvMsgSize = recv(clntSock, &bookBuffer, sizeof(Book), 0)) < 0)
                DieWithError("recv() failed");
        }

        library.books[library.num_books] = bookBuffer;
        ++library.num_books;
        bubbleSort(&library);
        close(clntSock);    /* Close client socket */
    }
}
