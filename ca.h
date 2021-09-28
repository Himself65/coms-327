#ifndef COM_S_327_P1__CA_H_
#define COM_S_327_P1__CA_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ca_data {
  int size;
  int *odca;
};

void display1DCA(struct ca_data *);
int set1DCACell(struct ca_data *, unsigned int, unsigned char);
void init1DCA(struct ca_data *, int);
struct ca_data *create1DCA(int, unsigned char);

typedef unsigned char (*StepFn)(struct ca_data *, int);

void stepCA(struct ca_data *, StepFn, int);

#endif //COM_S_327_P1__CA_H_
