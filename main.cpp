#include "GraphicsClient.h"
#include "CellularAutomaton.h"

unsigned char aRule(CellularAutomaton &ca, int x, int y) {
  int width = ca.width();
  int height = ca.height();
  int count = 0;
  if (ca[((x - 1) + width) % width][((y - 1) + height) % height] != 0) count++;  // left top
  if (ca[((x - 1) + width) % width][y] != 0) count++; // middle left
  if (ca[((x - 1) + width) % width][(y + 1) % height] != 0) count++; // left bottom
  if (ca[x][((y - 1) + height) % height] != 0) count++; // middle top
  if (ca[x][(y + 1) % height] != 0) count++; // center bottom

  if (ca[(x + 1) % width][((y - 1) + height) % height] != 0) count++; // right top
  if (ca[(x + 1) % width][y] != 0) count++; // right middle
  if (ca[(x + 1) % width][(y + 1) % height] != 0) count++; // right bottom
  int exit = 0;
  // check
  if (ca[x][y] == 0) {
    // dead
    if (count == 3) {
      exit = 1;
    }
  } else {
    if (count < 2) {
      exit = 0;
    } else if (count == 2 || count == 3) {
      exit = 1;
    } else if (count > 3) {
      exit = 0;
    }
  }
  //  printf("pos(%d, %d) %d %d %d from %d~> %d\n", ca->width, ca->height, x, y, count, ca->cadata[x][y], exit);
  return exit;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "incorrect length of arguments" << std::endl;
    return -1;
  }
  const std::string path(argv[1]);
  CellularAutomaton automaton(path, 0);

  const std::string address = "localhost";
  const int port = 7777;
  GraphicsClient client(address, port);
  automaton.connectClient(client);
  client.setBackgroundColor(0, 0, 0);
  client.setDrawingColor(255, 255, 255);
  client.clear();
  while (std::cin) {
    char ch;
    std::cin >> ch;
    if (ch == '\n') {
      automaton.step(aRule);
    } else {
      break;
    }
  }
  return 0;
}
