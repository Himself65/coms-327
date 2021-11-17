#ifndef Header_CellularAutomaton
#define Header_CellularAutomaton

#include <string>
#include <cstdio>
#include <iostream>
#include "GraphicsClient.h"

class CellularAutomaton {
 public:
  void connectClient(GraphicsClient &client);
  void step(unsigned char (CellularAutomaton &target, int x, int y));
  CellularAutomaton(const std::string &path,
                    int q_state);
  CellularAutomaton &operator=(const CellularAutomaton &that);
  ~CellularAutomaton();

  int width() const { return this->width_; }
  int height() const { return this->height_; }
  int size() const {
    const int m = std::max(this->width_, this->height_);
    if (m <= 50) {
      return 10;
    } else if (m <= 100) {
      return 4;
    } else if (m <= 200) {
      return 2;
    } else {
      return 1;
    }
  }
  int gap() const {
    const int m = std::max(this->width_, this->height_);
    if (m <= 50) {
      return 2;
    } else if (m <= 100) {
      return 1;
    } else if (m <= 200) {
      return 1;
    } else {
      return 0;
    }
  }

  int *operator[](int x);

 private:
  GraphicsClient *client_;
  std::string path_;
  FILE *file_;

  int q_state_;
  int width_;
  int height_;
  int **array_;
};

#endif
