#include "header.h"

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    // Указатель на файл.
    FILE *fpc;
    // Название файла ввода.
    char* filename;
    // Если название не указано в аргументах командной строки, осуществляется ввод из консоли.
    if (argc < 2) {
        printf("Enter the filename: ");
        scanf("%s", filename);
    } else {
        filename = argv[1];
    }

    // Открытие файла ввода.
    fpc = fopen(filename, "r");
    if (fpc == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    // Осуществление подсчета кол-ва позиций в файле.
    int a, b, c, d;
    int books_amount = 0;
    while (fscanf(fpc, "%d %d %d %d", &a, &b, &c, &d) != EOF) {
        ++books_amount;
    }
    // Закрытие файла.
    fclose(fpc);
    // Указатель на файл.
    FILE *fp;
    // Открываем файл
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    // Создание и заполнение каталога книгами.
    Catalog books;
    books.books = (Book*) malloc(books_amount);
    int iterator = 0;
    while (fscanf(fp, "%d %d %d %d", &books.books->row_number, &books.books->bookshelf_number, &books.books->book_numer, &books.books->id) != EOF) {
        printf("%d %d %d %d\n", books.books->row_number, books.books->bookshelf_number, books.books->book_numer, books.books->id);
        ++iterator;
    }

    fclose(fp);
    printf("Input done.\n");

    // Sending info to server.
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[sizeof(Book)];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv()
                                        and total bytes read */

    if ((argc < 3) || (argc > 4))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n",
               argv[0]);
       exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoString = argv[2];         /* Second arg: string to echo */

    if (argc == 4)
        echoServPort = atoi(argv[3]); /* Use given port, if any */
    else
        echoServPort = 7;  /* 7 is the well-known port for the echo service */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");

    echoStringLen = strlen(echoString);          /* Determine input length */

    for(int i = 0; i < 10; ++i) {
        /* Send the string to the server */
        if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
            DieWithError("send() sent a different number of bytes than expected");

        /* Receive the same string back from the server */
        totalBytesRcvd = 0;
        printf("%d) Received: ", i);            // Setup to print the echoed string
        while (totalBytesRcvd < echoStringLen) {
            /* Receive up to the buffer size (minus 1 to leave space for
            a null terminator) bytes from the sender */
            if ((bytesRcvd = recv(sock, echoBuffer, sizeof(Book) - 1, 0)) <= 0)
                DieWithError("recv() failed or connection closed prematurely");
            totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
            echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
            printf("%s", echoBuffer);      /* Print the echo buffer */
        }

        printf("\n");    /* Print a final linefeed */
        sleep(2);
    }

    close(sock);
    exit(0);
}
