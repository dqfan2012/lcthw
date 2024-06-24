#include <stdio.h>

void print_distance(int distance) {
  printf("You are %d miles away.\n", distance);
}

/* this is a comment */
#ifndef TESTING
int main(int argc, const char *argv[]) {
  int distance = 100;

  // this is also a comment
  print_distance(distance);

  return 0;
}
#endif
