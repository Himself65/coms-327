#include "dca1d.h"

/**
 *
 * @param size[in] the number of cells
 * @param pointer[in] a pointer to the start of the array
 * @returns 1DCA
 */
ODCA init1DCA(int size, unsigned char pointer[]) {
  memset(pointer, 0, size);
  ODCA dca = {
      .size = size,
      .p = pointer
  };
  return dca;
}

/**
 *
 * @param odca[in] a pointer to the start of the 1DCA array of unsigned characters,
 * @param pos[in] an index into that array
 * @param value[in] the state to set the indexed cell
 * @returns
 *  0 if everything runs correct,
 *  -1 if `pos` is a illegal number
 */
int set1DCACell(ODCA *odca, int pos, unsigned int value) {
  if (pos < 0) {
    // negative number is illegal
    return -1;
  }
  odca->p[pos] = value;
  return 0;
}

/**
 *
 * @param odca[in] the start of the 1DCA array
 * @param size[in] the number of cells in the 1DCA
 */
void display1DCA(ODCA *odca, int size) {
  for (int i = 0; i < size; ++i) {
    const unsigned char v = odca->p[size];
    printf("%c", v);
  }
  printf("\n");
}
