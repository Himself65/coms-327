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
  unsigned char *cells;
  int size;
  int total_state;
  int init_value;
};

void display1DCA(struct ca_data *);
int set1DCACell(struct ca_data *, unsigned int, unsigned char);
void init1DCA(struct ca_data *, int);
struct ca_data *create1DCA(int, unsigned char);

typedef unsigned char (*StepFn)(struct ca_data *, int);

void stepCA(struct ca_data *, StepFn, int);

#endif //COM_S_327_P1__CA_H_
