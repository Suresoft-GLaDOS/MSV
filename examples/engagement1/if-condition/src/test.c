#include <stdio.h>

void func(int a, int b){
  if (a == 1 && b == 1) { // (a == 1 && b == 1) || b == 2
    printf("%d\n", 0);
  } else {
    printf("%d\n", 1);
  }
}

int main(int argc, char *argv[]) {
  FILE *f = fopen(argv[1], "r");
  int a, b;
  fscanf(f, "%d %d\n", &a, &b);
  fclose(f);
  
  func(a, b);
  return 0;
}
