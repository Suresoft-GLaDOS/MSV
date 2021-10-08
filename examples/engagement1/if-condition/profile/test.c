#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *f = fopen(argv[1], "r");
  int a, b;
  fscanf(f, "%d %d\n", &a, &b);
  fclose(f);

  if (a > b) { // >=
    printf("%d\n", 0);
  } else {
    printf("%d\n", 1);
  }
  
  return 0;
}
