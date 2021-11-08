#include "ca.h"

void displayCA(struct ca_data *ca) {
  for (int x = 0; x < ca->width; x++) {
    for (int y = 0; y < ca->height; y++) {
      printf("%d%c", ca->cadata[x][y], x + 1 == ca->width ? '\n' : ' ');
    }
  }
}

void set_ca_intl(struct ca_data *ca, unsigned int x, unsigned int y, unsigned char v) {
  ca->cadata[x][y] = v;
}

int set1DCACell(struct ca_data *ca, unsigned int x, unsigned char state) {
  if (ca == NULL) {
    return 1;
  }
  if (x < 0 || x >= ca->width) {
    return -1;
  }
  set_ca_intl(ca, x, 0, state);
  return 0;
}

int set2DCACell(struct ca_data *ca, unsigned int x, unsigned int y, unsigned char state) {
  if (ca == NULL) {
    return 1;
  }
  if (x < 0 || x >= ca->width) {
    return -1;
  } else if (y < 0 || y >= ca->height) {
    return -1;
  }
  set_ca_intl(ca, x, y, state);
  return 0;
}

void initCA(struct ca_data *ca, int init_value) {
  if (ca == NULL) {
    return;
  }
  if (init_value == -1) {
    // random values
    time_t t;
    srand((unsigned) time(&t));
    for (int x = 0; x < ca->width; x++) {
      for (int y = 0; y < ca->height; y++) {
        set_ca_intl(ca, x, y, rand() % ca->total_state);
      }
    }
  } else {
    for (int x = 0; x < ca->width; x++) {
      for (int y = 0; y < ca->height; y++) {
        set_ca_intl(ca, x, y, init_value);
      }
    }
  }
}

static unsigned char **create_data_intl(int width, int height) {
  unsigned char **arr = malloc(sizeof(unsigned char *) * width);
  for (int x = 0; x < width; ++x) {
    arr[x] = malloc(sizeof(unsigned char) * height);
  }
  return arr;
}

struct ca_data *create1DCA(int width, unsigned char q_state) {
  if (width < 0) {
    return NULL;
  }
  struct ca_data *ca = malloc(sizeof(struct ca_data));
  if (ca == NULL) {
    return NULL;
  }
  ca->cadata = create_data_intl(width, 1);
  if (ca->cadata == NULL) {
    free(ca);
    return NULL;
  }
  ca->width = width;
  ca->height = 1;
  ca->dimension = 1;
  initCA(ca, q_state);
  return ca;
}

struct ca_data *create2DCA(int width, int height, unsigned char q_state) {
  if (width < 0 || height < 0) {
    return NULL;
  }
  struct ca_data *ca = malloc(sizeof(struct ca_data));
  if (ca == NULL) {
    return NULL;
  }
  ca->cadata = create_data_intl(width, height);
  if (ca->cadata == NULL) {
    free(ca);
    return NULL;
  }
  ca->width = width;
  ca->height = height;
  ca->dimension = 2;
  initCA(ca, q_state);
  return ca;
}

void step1DCA(struct ca_data *ca, unsigned char (*rule)(struct ca_data *, int x)) {
  if (ca == NULL) {
    return;
  }
  unsigned char **new_data = create_data_intl(ca->width, 1);
  for (int x = 0; x < ca->width; ++x) {
    new_data[x][0] = rule(ca, x);
  }
  for (int x = 0; x < ca->width; ++x) {
    ca->cadata[x][0] = new_data[x][0];
  }
}

void step2DCA(struct ca_data *ca, unsigned char (*rule)(struct ca_data *, int x, int y)) {
  if (ca == NULL) {
    return;
  }
  unsigned char **new_data = create_data_intl(ca->width, ca->height);
  for (int x = 0; x < ca->width; ++x) {
    for (int y = 0; y < ca->height; ++y) {
      new_data[x][y] = rule(ca, x, y);
    }
  }
  for (int x = 0; x < ca->width; ++x) {
    for (int y = 0; y < ca->height; ++y) {
      ca->cadata[x][y] = new_data[x][y];
    }
  }
}
