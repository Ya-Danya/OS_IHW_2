#include "header.h"

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    Book book_to_send;                /* String to send to echo server */
    unsigned int sizeOfBook = sizeof(Book);      /* Length of string to echo */
    char echoBuffer[sizeOfBook];     /* Buffer for echo string */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() */
    // Указатель на файл.
    FILE *fpc;
    // Название файла ввода.
    char* filename;
    // Если название не указано в аргументах командной строки, осуществляется ввод из консоли
    
    if (argc == 4) {
        echoServPort = atoi(argv[3]); /* Use given port, if any */
        filename = argv[1];
    } else {
        echoServPort = 7;  /* 7 is the well-known port for the echo service */
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

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */

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

    sizeOfBook = sizeof(Book);          /* Determine input length */

    iterator = 0;

    pid_t pid1 = fork();
    if (pid1 == -1) {
        printf("Ошибка при создании дочернего процесса\n");
        exit(-1);
    } else if (pid1 == 0) {
        pid_t pid2 = fork();
    
        if (pid1 == -1) {
            printf("Ошибка при создании дочернего процесса\n");
            exit(-1);
        } else if (pid2 == 0) {
            iterator = 2;
            if (books_amount > 1) {
                do {
                    if (send(sock, &books.books[iterator], sizeOfBook, 0) != sizeOfBook)
                    DieWithError("send() sent a different number of bytes than expected");
                    printf("\n");    /* Print a final linefeed */
                    iterator += 3;
                } while (iterator < books_amount);
        
                close(sock);
                exit(0);
            }
        } else {
            iterator = 1;
            if (books_amount > 0) {
                do {
                    if (send(sock, &books.books[iterator], sizeOfBook, 0) != sizeOfBook)
                    DieWithError("send() sent a different number of bytes than expected");
                    printf("\n");    /* Print a final linefeed */
                    iterator += 3;
                } while (iterator < books_amount);
        
                close(sock);
                exit(0);
            }
        }
    } else {
        iterator = 0;
        do {
            if (send(sock, &books.books[iterator], sizeOfBook, 0) != sizeOfBook)
                DieWithError("send() sent a different number of bytes than expected");
            printf("\n");    /* Print a final linefeed */
            iterator += 3;
        } while (iterator < books_amount);
        
        close(sock);
        exit(0);
    }
}
