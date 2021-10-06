#include "csapp.h"

void echo(int connfd) {

    rio_t rio;
    char buf[MAXLINE];
    size_t n;

    Rio_readinitb(&rio, connfd);
    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %d bytes\n", (int) n);
        Rio_writen(connfd, buf, n);
    }
}
