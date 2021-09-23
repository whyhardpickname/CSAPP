#include "csapp.h" 

int main(int argc, char const *argv[])
{
    struct in_addr inaddr;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <dotted decimal>\n", argv[0]);
        exit(0);
    }
    

    int result = inet_pton(AF_INET, argv[1], &inaddr);
    if (result == 0) {
        app_error("illegal command line argument.\n");
        exit(0);
    }
    else if (result != 1) {
        unix_error("inet_pton.\n");
        exit(0);
    }

    printf("0x%x\n", ntohl(inaddr.s_addr));
    return 0;
}
