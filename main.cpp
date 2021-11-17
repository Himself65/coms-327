#include "GraphicsClient.h"
#include "CellularAutomaton.h"

unsigned char aRule(CellularAutomaton &ca, int x, int y) {
  const int width = ca.width();
  const int height = ca.height();
  const int lx = (x + width - 1) % width;
  const int rx = (x + 1) % width;

  const int uy = (y + height - 1) % height;
  const int ly = (y + 1) % height;

  int count = 0;
  if (ca[lx][uy] != 0) count++;  // left top
  if (ca[lx][y] != 0) count++; // middle left
  if (ca[lx][ly] != 0) count++; // left bottom
  if (ca[x][uy] != 0) count++; // middle top
  if (ca[x][ly] != 0) count++; // center bottom

  if (ca[rx][uy] != 0) count++; // right top
  if (ca[rx][y] != 0) count++; // right middle
  if (ca[rx][ly] != 0) count++; // right bottom
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
  //  printf("pos(%d, %d) %d from %d~> %d\n", x, y, count, ca[x][y], exit);
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
  client.clear();
  client.repaint();
  client.setBackgroundColor(0, 0, 0);
  client.setDrawingColor(255, 255, 255);
  client.clear();
  client.repaint();
  int step = 0;
  while (true) {
    int ch = getchar();
    if (ch == '\n') {
//      std::cout << "step: " << step << std::endl;
      step++;
      automaton.step(aRule);
    } else {
      std::cout << "exit" << std::endl;
      break;
    }
  }
  return 0;
}
