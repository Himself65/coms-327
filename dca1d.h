#ifndef COM_S327_P1__DCA1D_H_
#define COM_S327_P1__DCA1D_H_
#include <stdio.h>
#include <string.h>

typedef struct {
  unsigned char *p;
  int size;
} ODCA;

ODCA init1DCA(int size, unsigned char pointer[]);
int set1DCACell(ODCA odca[], int pos, unsigned int value);
void display1DCA(ODCA odca[], int size);

#endif //COM_S327_P1__DCA1D_H_
