#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";
static const char *conn_hdr = "Connection: close\r\n";
static const char *prox_hdr = "Proxy-Connection: close\r\n";

void doit(int fd);
void create_requesthdrs(rio_t *rp, char *request, char *hostname, char *port);
int parse_uri(char *uri, char *hostname, char *port, char *path);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char *argv[]) {
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  /* 检查命令行参数 */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
    printf("Accept connection from (%s, %s)\n", hostname, port);
    doit(connfd);
    Close(connfd);
  }
}

void doit(int fd) {

  int is_valid, clientfd, n;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char hostname[MAXLINE], port[MAXLINE], path[MAXLINE];
  char request[MAXLINE], hdrs[MAXLINE], forward_buf[MAXLINE];
  rio_t rio_client, rio_server;

  /* 读取请求行和请求头 */
  Rio_readinitb(&rio_client, fd);
  Rio_readlineb(&rio_client, buf, MAXLINE);
  sscanf(buf, "%s %s %s", method, uri, version);
  if (strcasecmp("GET", method)) {
    clienterror(fd, method, "501", "Not implemented",
                "Tiny does not implement this method");
    return;
  }

  /* 解析GET request的uri */
  is_valid = parse_uri(uri, hostname, port, path);

  if (is_valid) {
    clientfd = Open_clientfd(hostname, port);
    //添加方法，路径和协议
    sprintf(request, "%s %s HTTP/1.0\r\n", method, path);
    //添加请求头
    create_requesthdrs(&rio_client, request, hostname, port);
    //发送请求
    Rio_writen(clientfd, request, sizeof(request));
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

void create_requesthdrs(rio_t *rp, char *request, char *hostname, char *port) {  
  char buf[MAXLINE];

  while (Rio_readlineb(rp, buf, MAXLINE) > 0) {
    if (!strcmp(buf, "\r\n")) break;
    if (strstr(buf,"Host:") != NULL) continue;
    if (strstr(buf,"User-Agent:") != NULL) continue;
    if (strstr(buf,"Connection:") != NULL) continue;
    if (strstr(buf,"Proxy-Connection:") != NULL) continue;

    sprintf(request, "%s%s", request, buf);
  }

  sprintf(request, "%sHost: %s:%s\r\n", request, hostname, port);
  sprintf(request, "%s%s", request, user_agent_hdr);
  sprintf(request, "%s%s", request, conn_hdr);
  sprintf(request, "%s%s", request, prox_hdr);
  sprintf(request, "%s\r\n", request);
}

int parse_uri(char *uri, char *hostname, char *port, char *path) {
  char temp[MAXLINE];
  if (sscanf(uri, "http://%[^/]%s", temp, path) != 2) {
    return 0;
  }

  if (strstr(temp, ":")) {
    if (sscanf(temp, "%[^:]:%[0-9]", hostname, port) != 2) {
      return 0;
    }
  }
  else {
    memcpy(hostname, temp, sizeof(temp));
    port = "80";
  }
  return 1;
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