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
  int *cells = malloc(sizeof(int) * (size + 2));
  if (cells == NULL) {
    return NULL;
  }
  for (int i = 0; i < size + 2; ++i) {
    cells[i] = init_value;
  }
  ca->odca = cells + 1; // allow the ca->odca[-1] operation
  ca->size = size;
  ca->init_value = init_value;
  return ca;
}

void stepCA(struct ca_data *ca, StepFn fn, int flag) {
  if (ca == NULL) {
    return;
  }
  int *new_states = malloc(sizeof(int) * ca->size);
  if (new_states == NULL) {
    return;
  }
  if (flag) {
    // wrapped around to the other edge of the cellular automata
    ca->odca[-1] = ca->odca[ca->size - 1];
    ca->odca[ca->size] = ca->odca[0];
  } else {
    // otherwise, the edge values equal to the initial value
    ca->odca[-1] = ca->init_value;
    ca->odca[ca->size] = ca->init_value;
  }
  for (int i = 0; i < ca->size; ++i) {
    int new_value = fn(ca, i);
    new_states[i] = new_value;
  }
  // set the new value to the original
  for (int i = 0; i < ca->size; ++i) {
    ca->odca[i] = new_states[i];
  }
}
