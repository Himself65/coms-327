#ifndef Header_CellularAutomaton
#define Header_CellularAutomaton

#include <string>
#include <fstream>
#include "GraphicsClient.h"

class CellularAutomaton {
 public:
  void connectClient(GraphicsClient &client);
  void step(unsigned char (CellularAutomaton &target, int x, int y));
  CellularAutomaton(std::string path,
                    int q_state);
  CellularAutomaton &operator=(const CellularAutomaton &that);
  ~CellularAutomaton();

  int width() const { return this->width_; }
  int height() const { return this->height_; }

  int *operator[](int x);

 private:
  GraphicsClient *client_;
  std::string path_;
  std::ifstream infile_;

  int q_state_;
  int width_{};
  int height_{};
  int **array_;
};

#endif
