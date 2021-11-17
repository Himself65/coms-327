#ifndef Header_CellularAutomaton
#define Header_CellularAutomaton

#include <string>
#include <cstdio>
#include <iostream>
#include "GraphicsClient.h"

class CellularAutomaton {
 public:
  /**
   * takes a single reference parameter to a GraphicsClient object and
   * display the 2DCA on the attached graphics window associated with the GraphicsClient object.
   * @param client a single reference parameter to a GraphicsClient object
   */
  void connectClient(GraphicsClient &client);

  /**
   * takes a single argument that is the rule function and performs one step of the 2DCA.
   * @param callback a function pointer
   */
  void step(unsigned char callback(CellularAutomaton &target, int x, int y));

  /**
   *
   * @param path path to the input
   * @param q_state quiescent state of the CA
   */
  CellularAutomaton(const std::string &path,
                    int q_state);
  CellularAutomaton &operator=(const CellularAutomaton &that);
  ~CellularAutomaton();

  /**
   * @return the width of the CA
   */
  int width() const { return this->width_; }
  /**
   * @return the height of the CA
   */
  int height() const { return this->height_; }
  /**
   * @return the max size of the CA
   */
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
  /**
   * @return the gap between each cell
   */
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
