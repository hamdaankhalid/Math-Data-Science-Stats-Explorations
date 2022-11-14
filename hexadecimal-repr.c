#include <stdio.h>

void show_bytes(char *start, size_t len) {
  printf("%.2x \n", start);
  for (int i = 0; i < len; i++)
  {
    printf("%.2x", start[i]);
  }
}

int main() {
  float i = 12345.0;
  show_bytes((char *)&i, sizeof(float));
  return 0;
}