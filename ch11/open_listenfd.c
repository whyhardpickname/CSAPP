#include "csapp.h"

int open_listenfd(char *port);

int open_listenfd(char *port) {
    
    int clientfd, opval = 1;
    struct addrinfo hints, *listp, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;
    Getaddrinfo(NULL, port, &hints, &listp);

    for (p = listp; p; p = p-> ai_next) {
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            continue;
        }

        Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opval, sizeof(int));

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) {
            break;
        }
        Close(listenfd);
    }

    if (!p) {
        return -1;
    }

    if (listen(listenfd, LISTENQ) < 0) {
        Close(listenfd);
        return -1;
    }
    return listenfd;
}