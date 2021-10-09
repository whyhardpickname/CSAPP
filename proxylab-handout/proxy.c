#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *hostname, char *port, char *path);
void get_filetype(char *filename, char *filetype);
void serve_static(int fd, char *filename, int filesize);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char *argv[]) {
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  /* 检查命令行参数 */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[1]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE,
                0);
    printf("Accept connection from (%s, %s)\n", hostname, port);
    doit(connfd);
    Close(connfd);
  }
}

void doit(int fd) {

  int is_valid, clientfd, n;
  struct stat sbuf;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char hostname[MAXLINE], port[MAXLINE], path[MAXLINE];
  char url[MAXLINE], request_headers[MAXLINE], forward_buf[MAXLINE];
  rio_t rio, rio_server;

  /* 读取请求行和请求头 */
  rio_readinitb(&rio, fd);
  rio_readlineb(&rio, buf, MAXLINE);
  printf("Request headers:\n");
  printf("%s", buf);
  sscanf(buf, "%s %s %s", method, uri, version);
  if (strcasecmp("GET", method)) {
    clienterror(fd, method, "501", "Not implemented",
                "Tiny does not implement this method");
    return;
  }
  read_requesthdrs(&rio);

  /* 解析GET request的uri */
  is_valid = parse_uri(uri, hostname, port, path);

  if (is_valid) {
    clientfd = Open_clientfd(hostname, port);
    //发送uri
    sprintf(url, "%s %s HTTP/1.0\r\n\r\n", method, path);
    Rio_writen(clientfd, url, sizeof(url));
    //发送请求头
    sprintf(request_headers, "Host: %s\r\n", hostname);
    sprintf(request_headers, "%s", user_agent_hdr);
    sprintf(request_headers, "Connection: close\r\n");
    sprintf(request_headers, "Proxy-Connection: close\r\n");
    Rio_writen(clientfd, request_headers, sizeof(request_headers));
    //转发读取内容
    Rio_readinitb(&rio_server, clientfd);
    while((n = rio_readnb(&rio_server, forward_buf, MAXLINE)) >= 0) {
      Rio_writen(fd, forward_buf, n);
    }

  }
  else {
    clienterror(fd, uri, "400", "Bad Request", "invalid hostname or port.");
  }
}

void read_requesthdrs(rio_t *rp) {
  
  char buf[MAXLINE];

  Rio_readlineb(rp, buf, MAXLINE);
  while (strcmp(buf, "\r\n")) {
    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
  }
}

int parse_uri(char *uri, char *hostname, char *port, char *path) {

  int i;
  sscanf(uri, "http://%s:%d%s", hostname, &i, path);
  sscanf(uri, "http://%[^/]%s", hostname, path);

  
}

/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
/* $end clienterror */