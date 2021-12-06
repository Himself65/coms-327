#ifndef Header_CellularAutomaton
#define Header_CellularAutomaton

#include <string>
#include <cstdio>
#include <iostream>
#include "GraphicsClient.h"

struct Position {
  int x;
  int y;
  int width;
  int height;
};

struct Type {
  int size;
  int gap;
};

static Type
    Large = {10, 4},
    Middle = {4, 1},
    Small = {1, 1};

enum class Mode {
  Running,
  Pause,
  Quit,
};

class CellularAutomaton {
 public:
  typedef void (*Callback)();
  class ButtonScope {
   public:
    ButtonScope(CellularAutomaton *cellular_automaton,
                const Position &position,
                const std::string &text) {
      auto *client = cellular_automaton->client_;
      if (client != nullptr) {
        client->drawRectangle(position.x, position.y, position.width, position.height);
        client->drawstring(position.x + 10, position.y + (position.height / 2), text);
        cellular_automaton->buttons_.emplace_back(Position(position), std::string(text));
        std::cout << "init " << text << " success!" << std::endl;
      } else {
        std::cerr << "client is nullptr" << std::endl;
      }
    }
    ~ButtonScope() = default;
   private:
  };
  /**
   * takes a single reference parameter to a GraphicsClient object and
   * display the 2DCA on the attached graphics window associated with the GraphicsClient object.
   * @param client a single reference parameter to a GraphicsClient object
   */
  void connectClient(GraphicsClient &client);

  void loadFile(const std::string &path);

  void simulate();

  /**
   *
   * @param path path to the input
   * @param q_state quiescent state of the CA
   */
  CellularAutomaton();
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
    return this->type_.size;
  }
  /**
   * @return the gap between each cell
   */
  int gap() const {
    return this->type_.gap;
  }

  int *operator[](int x);

 private:
  void step();
  void run();
  void pause();
  void reset();
  void random();
  void load();
  void quit();
  void select_size(int size);

  GraphicsClient *client_;
  std::string path_;
  FILE *file_{};

  std::vector<std::pair<Position, std::string>> buttons_;
  Mode mode_;
  Type &type_;

  int width_{};
  int height_{};

  int **init_array_{};
  int **array_{};
};

#endif
