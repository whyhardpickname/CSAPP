#include <stdio.h>
int conditional(int x, int y, int z) {
  int i = !x;
  printf("%d\n", i);
  int mask = (~i) + 1;
  printf("%x\n", mask);
  return ((!mask) & y) | (mask & z);
}
int main()
{
    printf("%d\n", conditional(4, 5, 1));
    return 0;
}