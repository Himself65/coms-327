#include "ca.h"
const int k_size = 25;

int in_range(const int input) {
  const int max_num = 100;
  const int min_num = 10;
  if (input >= min_num && input <= max_num) {
    return 1;
  } else {
    return 0;
  }
}

unsigned char aRule(struct ca_data *ca, int index) {
  // todo
  ca->odca[index]++;
  return ca->odca[index];
}

int main() {
  struct ca_data *ca = create1DCA(k_size, 1);
  display1DCA(ca);
  init1DCA(ca, 2);
  display1DCA(ca);
  set1DCACell(ca, 2, 3);
  display1DCA(ca);
  return 0;
}
