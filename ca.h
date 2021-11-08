#ifndef COM_S_327_P1__CA_H_
#define COM_S_327_P1__CA_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifndef NULL
#define NULL  ((void *)0)
#endif

struct ca_data {
  unsigned char **cadata;  // original data
  unsigned char dimension; // 1 or 2 dimension(s)
  unsigned char wrap; // 0 for nowrap, 1 for wrap
  int width;
  int height; // for 2 dimensions
  int total_state;
};

void displayCA(struct ca_data *ca);
int set1DCACell(struct ca_data *ca, unsigned int x, unsigned char state);
int set2DCACell(struct ca_data *ca, unsigned int x, unsigned int y, unsigned char state);
void initCA(struct ca_data *ca, int state);
// x, y are index for the cell
void step1DCA(struct ca_data *ca, unsigned char (*rule)(struct ca_data *, int x));
void step2DCA(struct ca_data *ca, unsigned char (*rule)(struct ca_data *, int x, int y));
// width, h give the size of the CA
struct ca_data *create1DCA(int width, unsigned char q_state);
struct ca_data *create2DCA(int w, int h, unsigned char q_state);

#endif //COM_S_327_P1__CA_H_
