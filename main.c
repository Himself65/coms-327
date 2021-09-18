#include "dca1d.h"
const int k_size = 25;

int main() {
  unsigned char p[k_size];
  ODCA odca = init1DCA(k_size, p);
  for (int i = 0; i < 25; ++i) {
    if (i % 2 == 0) {
      // even number
      set1DCACell(&odca, i, 2);
    } else {
      // odd number
      set1DCACell(&odca, i, 1);
    }
  }
  display1DCA(&odca, k_size);
  return 0;
}
