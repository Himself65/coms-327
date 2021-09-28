#include "ca.h"

void display1DCA(struct ca_data *ca) {
  for (int pos = 0; pos < ca->size; pos++) {
    printf("%d%c", ca->odca[pos], pos + 1 == ca->size ? '\n' : ' ');
  }
}

int set1DCACell(struct ca_data *ca, unsigned int pos, unsigned char value) {
  if (ca == NULL) {
    return 1;
  }
  if (pos < 0 || pos > ca->size) {
    return -1;
  }
  ca->odca[pos] = value;
  return 0;
}
void init1DCA(struct ca_data *ca, int init_value) {
  if (ca == NULL) {
    return;
  }
  for (int i = 0; i < ca->size; i++) {
    ca->odca[i] = init_value;
  }
}

struct ca_data *create1DCA(int size, unsigned char init_value) {
  struct ca_data *ca = malloc(sizeof(struct ca_data));
  if (ca == NULL) {
    return NULL;
  }
  if (size < 0) {
    return NULL;
  }
  int *cells = malloc(sizeof(int) * size);
  if (cells == NULL) {
    return NULL;
  }
  for (int i = 0; i < size; ++i) {
    cells[i] = init_value;
  }
  ca->odca = cells;
  ca->size = size;
  return ca;
}

void stepCA(struct ca_data *ca, StepFn fn, int value) {
  for (int i = 0; i < ca->size; ++i) {
    fn(ca, i);
  }
}
