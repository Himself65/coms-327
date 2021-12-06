#include "GraphicsClient.h"
#include "CellularAutomaton.h"

int main(int argc, char *argv[]) {
  CellularAutomaton automaton;
  const std::string address = "localhost";
  const int port = 7777;
  GraphicsClient client(address, port);
  client.clear();
  client.repaint();
  client.setBackgroundColor(0, 0, 0);
  client.setDrawingColor(255, 255, 255);
  client.clear();
  client.repaint();
  automaton.connectClient(client);
  automaton.simulate();
  return 0;
}
