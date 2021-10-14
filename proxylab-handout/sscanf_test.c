#include <stdio.h>
#include <string.h>

#define MAXLINE 128

int parse_uri(char *uri, char *hostname, char *port, char *path);

int main(int argc, char const *argv[]) {
  char *uri1 = "http://www.cmu.edu:80/hub/index.html";
  char *uri2 = "http://www.cmu.edu/hub/index.html";
  char ip[MAXLINE], hostname[MAXLINE], path[MAXLINE], port[MAXLINE];
  int n;

  n = parse_uri(uri1, hostname, port, path);
  printf("%d\n", n);
  printf("%s\n", hostname);
  printf("%s\n", port);
  printf("%s\n", path);

  n = parse_uri(uri2, hostname, port, path);
  printf("%d\n", n);
  printf("%s\n", hostname);
  printf("%s\n", port);
  printf("%s\n", path);

  return 0;
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
    if (sscanf(temp, "%s", hostname) != 1) {
      return 0;
    }
    port = "80";
  }
  return 1;
}