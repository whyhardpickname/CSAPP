#include "csapp.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <domain name>\n", argv[0]);
        exit(0);
    }

    struct addrinfo *p, *listp, hints;
    char buf[MAXLINE];
    int rc, flags;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s \n", gai_strerror(rc));
        exit(1);
    }

    flags = NI_NUMERICHOST;
    struct sockaddr_in *sockp;
    for (p = listp; p; p = p->ai_next) {
        sockp = (struct sockaddr_in *) p->ai_addr;
        Inet_ntop(AF_INET, &(sockp->sin_addr), buf, MAXLINE);
        printf("%s\n", buf);
    }

    Freeaddrinfo(listp);
    
    exit(0);    
}
