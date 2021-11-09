#include "ca.h"
#include <stdio.h>

unsigned char aRule(struct ca_data *ca, int x, int y) {
  int width = ca->width;
  int height = ca->height;
  int count = 0;
  if (ca->cadata[((x - 1) + width) % width][y] != 0) count++; // middle left
  if (ca->cadata[((x - 1) + width) % width][((y - 1) + height) % height] != 0) count++;  // left top
  if (ca->cadata[x][((y - 1) + height) % height] != 0) count++; // middle top
  if (ca->cadata[(x + 1) % width][((y - 1) + height) % height] != 0) count++; // right top
  if (ca->cadata[(x + 1) % width][y] != 0) count++; // right middle
  if (ca->cadata[(x + 1) % width][(y + 1) % height] != 0) count++; // right bottom
  if (ca->cadata[x][(y + 1) % height] != 0) count++; // center bottom
  if (ca->cadata[((x - 1) + width) % width][(y + 1) % height] != 0) count++; // left bottom
  // check
  if (ca->cadata[x][y] == 0) {
    // dead
    if (count == 3) {
      return 1;
    }
  } else {
    if (count < 2) {
      return 0;
    } else if (count == 2 || count == 3) {
      return 1;
    } else if (count > 3) {
      return 0;
    }
  }
}

char buf[50];
int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("incorrect parameters length");
    return -1;
  }
  // step 1
  int dimension = atoi(argv[1]);
  if (dimension != 1 && dimension != 2) {
    printf("incorrect dimension");
    return -1;
  }
  // step 2
  const char *path = argv[2];
  FILE *file = fopen(path, "r");
  if (file == NULL) {
    printf("incorrect file path");
    return -1;
  }
  int width = 0;
  int height = 0;
  // width
  fscanf(file, "%d", &width);
  // height
  fscanf(file, "%d", &height);
  struct ca_data *ca = create2DCA(width, height, 0);
  if (ca == NULL) {
    return -1;
  }
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      unsigned int value;
      fscanf(file, "%u", &value);
      set2DCACell(ca, x, y, value);
    }
  }
  ca->wrap = 1;
  while (getchar() == '\n') {
    step2DCA(ca, aRule);
  }
  free(ca);
  fclose(file);
  return 0;
}
