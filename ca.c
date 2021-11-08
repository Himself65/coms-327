#include "ca.h"

void display1DCA(struct ca_data *ca) {
  for (int pos = 0; pos < ca->size; pos++) {
    printf("%d%c", ca->cadata[pos], pos + 1 == ca->size ? '\n' : ' ');
  }
}

int set1DCACell(struct ca_data *ca, unsigned int pos, unsigned char value) {
  if (ca == NULL) {
    return 1;
  }
  if (pos < 0 || pos > ca->size) {
    return -1;
  }
  ca->cadata[pos] = value;
  return 0;
}

void init1DCA(struct ca_data *ca, int init_value) {
  if (ca == NULL) {
    return;
  }
  if (init_value == -1) {
    // random values
    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < ca->size; i++) {
      set1DCACell(ca, i, rand() % ca->total_state);
    }
  } else {
    for (int i = 0; i < ca->size; i++) {
      set1DCACell(ca, i, init_value);
    }
  }
  ca->init_value = init_value;
}

struct ca_data *create1DCA(int size, unsigned char init_value) {
  struct ca_data *ca = malloc(sizeof(struct ca_data));
  if (ca == NULL) {
    return NULL;
  }
  if (size < 0) {
    return NULL;
  }
  unsigned char *cells = malloc(sizeof(unsigned char) * (size + 2));
  if (cells == NULL) {
    return NULL;
  }
  ca->cadata = cells + 1; // allow the ca->cadata[-1] and ca->cadata[size] operation
  ca->size = size;
  ca->init_value = init_value;
  init1DCA(ca, init_value);
  return ca;
}

void stepCA(struct ca_data *ca, StepFn fn, int flag) {
  if (ca == NULL) {
    return;
  }
  if (flag) {
    // wrapped around to the other edge of the cellular automata
    ca->cadata[-1] = ca->cadata[ca->size - 1];
    ca->cadata[ca->size] = ca->cadata[0];
  } else {
    // otherwise, the edge values equal to the initial value
    if (ca->init_value == -1) {
      ca->cadata[-1] = rand() % ca->total_state;
      ca->cadata[ca->size] = rand() % ca->total_state;
    } else {
      ca->cadata[-1] = ca->init_value;
      ca->cadata[ca->size] = ca->init_value;
    }
  }
  unsigned char *new_states = malloc(sizeof(unsigned char) * ca->size);
  if (new_states == NULL) {
    printf("error in stepCA, cannot malloc the memory\n");
    return;
  }
  for (int i = 0; i < ca->size; ++i) {
    int new_value = fn(ca, i);
    new_states[i] = new_value;
  }
  // set the new value to the original
  for (int i = 0; i < ca->size; ++i) {
    ca->cadata[i] = new_states[i];
  }
  free(new_states);
}
