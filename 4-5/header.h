#include <stdio.h>      /* for printf() and fprintf() */
#include <unistd.h>     /* for close() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket);

typedef struct {
    int row_number; // номер ряда
    int bookshelf_number; // номер шкафа
    int book_numer; // номер книги в шкафу
    int id; // инвентарный номер книги
} Book;

typedef struct {
    Book* books;
    int num_books;
} Catalog;
