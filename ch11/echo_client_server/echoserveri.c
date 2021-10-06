#include "csapp.h"

void echo(int connfd);

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(0);
    }

    char *port = argv[1], client_hostname[MAXLINE], client_port[MAXLINE];
    int listendfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    listendfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listendfd, (SA *) &clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        echo(connfd);
        Close(connfd);
    }

    exit(0);
}
