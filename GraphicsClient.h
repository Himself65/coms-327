#ifndef Header_GraphicsClient
#define Header_GraphicsClient

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

enum class Mnemonic {
  CLEAR = 1,
  SET_BACKGROUND_COLOR = 2,
  SET_PIXEL = 3,
  DRAW_STRING = 5,
  SET_DRAWING_COLOR = 6,
  DRAW_RECTANGLE = 7,
  FILL_RECTANGLE = 8,
  CLEAR_RECTANGLE = 9,
  DRAW_OVAL = 10,
  FILL_OVAL = 11,
  REPAINT = 12,
  DRAW_LINE = 13
};

class GraphicsClient {
 public:
  class MessageChannel {
   public:
    MessageChannel(GraphicsClient *client, Mnemonic type) : client_(client), message_(std::vector<char>()) {
      this->message_.push_back(static_cast<char>(type));
    }
    void push_int(int value) {
      this->message_.push_back(static_cast<char>((value >> 12) & 0xf));
      this->message_.push_back(static_cast<char>((value >> 8) & 0xf));
      this->message_.push_back(static_cast<char>((value >> 4) & 0xf));
      this->message_.push_back(static_cast<char>(value & 0xf));
    }
    void push_char(char ch) {
      this->message_.push_back(static_cast<char>((ch >> 4) & 0xf));
      this->message_.push_back(static_cast<char>(ch & 0xf));
    }
    void push_string(const std::string &string) {
      for (char const &ch: string) {
        this->push_char(ch);
      }
    }
    void push_color(int red, int green, int blue) {
      this->push_char(static_cast<char>(red));
      this->push_char(static_cast<char>(green));
      this->push_char(static_cast<char>(blue));
    }
    ~MessageChannel() {
      client_->sendMessage(this->message_);
    }
   private:
    GraphicsClient *client_;
    std::vector<char> message_;
  };

  /**
   * sets the background color of the associated display.
   * @param r red
   * @param g green
   * @param b blue
   */
  void setBackgroundColor(int r, int g, int b);

  /**
   * set the color that objects will be drawn at until another setDrawingColor call is made.
   * @param r red
   * @param g green
   * @param b blue
   */
  void setDrawingColor(int r, int g, int b);

  /**
   * clears the display to the background color.
   */
  void clear();

  /**
   * sets the pixel at the location given by the first two parameters to the color given by the last three parameters.
   * @param x
   * @param y
   * @param r red
   * @param g green
   * @param b blue
   */
  void setPixel(int x, int y, int r, int g, int b);

  void drawRectangle(int x, int y, int w, int h);

  void fillRectangle(int x, int y, int w, int h);

  void clearRectangle(int x, int y, int w, int h);
  void drawOval(int x, int y, int w, int h);
  void fillOval(int x, int y, int w, int h);

  void drawLine(int x1, int y1, int x2, int y2);
  void drawstring(int x, int y, const std::string &str);

  /**
   * send the redraw (repaint) signal to the attached graphics server.
   */
  void repaint();

  GraphicsClient(std::string url, int port);
  GraphicsClient(GraphicsClient &that);
  ~GraphicsClient();
  GraphicsClient &operator=(const GraphicsClient &that);

 private:

  void sendMessage(std::vector<char> message) const;
  void connectServer();
  void disconnectServer();
  std::string url_;
  int port_;
  int server_fd_;
};

#endif
