#include "ca.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int in_range(const int input) {
  const int max_num = 100;
  const int min_num = 10;
  if (input >= min_num && input <= max_num) {
    return 0;
  } else {
    return 1;
  }
}

unsigned char aRule(struct ca_data *ca, int index) {
  if (ca->odca[index - 1] == 0) {
    if (ca->odca[index] == 0) {
      if (ca->odca[index + 1] == 0) {
        // 000
        return 0;
      } else {
        // 001
        return 1;
      }
    } else {
      if (ca->odca[index + 1] == 0) {
        // 010
        return 1;
      } else {
        // 011
        return 1;
      }
    }
  } else {
    if (ca->odca[index] == 0) {
      if (ca->odca[index + 1] == 0) {
        // 100
        return 0;
      } else {
        // 101
        return 1;
      }
    } else {
      if (ca->odca[index + 1] == 0) {
        // 110
        return 1;
      } else {
        // 111
        return 0;
      }
    }
  }
}

int main() {
  // step 1
  int size; // size of the cell
  printf("please input the size of the 1DCA.\n");
  scanf("%d", &size);
  if (in_range(size) != 0) {
    printf("incorrect size");
    return -1;
  }
  // step 2
  int state;
  printf("number of states for each cell.\n");
  scanf("%d", &state);

  // step 3
  char flag_str[7];
  int flag;
  printf("input the flag(\"wrap\" or \"or wrap\").\n");
  scanf("%s", flag_str);
  if (strcmp(flag_str, "wrap") == 0) {
    flag = 1;
  } else if (strcmp(flag_str, "nowrap") == 0) {
    flag = 0;
  }
  // step 4
  int init_state = 0;
  printf("input the init state.\n");
  scanf("%d", &init_state);
  struct ca_data *ca = NULL;
  if (init_state > 0 && init_state < state) {
    // specific values
    ca = create1DCA(size, init_state);
  } else if (init_state == -1) {
    // random values
    time_t t;
    srand((unsigned) time(&t));
    ca = create1DCA(size, 0);
    for (int i = 0; i < ca->size; ++i) {
      set1DCACell(ca, i, rand() % state);
    }
  } else {
    printf("incorrect input init state\n");
    return -1;
  }
  // step 5
  int step;
  printf("input the step of the ca\n");
  scanf("%d", &step);
  display1DCA(ca);
  for (int i = 0; i < step; ++i) {
    // loop for emulate
    stepCA(ca, aRule, flag);
    display1DCA(ca);
  }
  return 0;
}
