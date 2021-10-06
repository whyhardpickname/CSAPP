#include "csapp.h"

int open_clientfd(char *hostname, char *port);

int open_clientfd(char *hostname, char *port)
{
    int clientfd;
    struct addrinfo hints, *listp, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    Getaddrinfo(hostname, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next) {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->protocol)) < 0) {
            continue;
        }

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
            break;
        }
        Close(clientfd);
    }

    Freeaddrinfo(listp);
    if (!p) {
        return -1;
    }
    
    return clientfd;
}