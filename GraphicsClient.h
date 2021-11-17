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
  CLEAR = 1, // Clear the display and set every pixel to the background color
  SET_BACKGROUND_COLOR = 2, // Set the background color to the specified color
  SET_PIXEL = 3,  // Set the specified pixel location to the specified color
  DRAW_STRING = 5,  // Draw the specified sting at a specified location
  SET_DRAWING_COLOR = 6,  // Set the drawing color for text, shapes, and pixels
  DRAW_RECTANGLE = 7, // Draw the specified rectangle at the specified location
  FILL_RECTANGLE = 8, // Draw and fill the specified rectangle at the specified location
  CLEAR_RECTANGLE = 9,  // Clear the specified rectangle at the specified location
  DRAW_OVAL = 10, // Draw the specified oval at the specified location
  FILL_OVAL = 11, // Draw and fill the specified oval at the specified location
  REPAINT = 12, // Cause the graphics server to update the display
  DRAW_LINE = 13  // Draw the specified line
};

class GraphicsClient {
 public:
  /**
   * MessageChannel is a helper class to send message to the server.
   * Using destructor, you don't need to worry about call the repeated functions.
   * And it will also auto calculate the length of the message
   */
  class MessageChannel {
   public:
    MessageChannel(GraphicsClient *client, Mnemonic type)
        : client_(client), length(1), message_(std::vector<char>()) {
      this->message_.push_back(static_cast<char>(0xff));
      this->message_.push_back(0);  // replace the length
      this->message_.push_back(0);
      this->message_.push_back(0);
      this->message_.push_back(0);
      this->message_.push_back(static_cast<char>(type));
    }
    /**
     * push an integer to the message, which length is 4;
     * @param value
     */
    void push(int value) {
      this->message_.push_back(static_cast<char>((value >> 12) & 0xf));
      this->message_.push_back(static_cast<char>((value >> 8) & 0xf));
      this->message_.push_back(static_cast<char>((value >> 4) & 0xf));
      this->message_.push_back(static_cast<char>(value & 0xf));
      this->length += 4;
    }
    /**
     * push a char to the message, which length is 2;
     * @param item
     */
    void push(char ch) {
      this->message_.push_back(static_cast<char>((ch >> 4) & 0xf));
      this->message_.push_back(static_cast<char>(ch & 0xf));
      this->length += 2;
    }
    /**
     * push a string to the message, auto calculate the length
     * @param string
     */
    void push(const std::string &string) {
      for (char const &ch: string) {
        this->push(ch);
      }
    }
    /**
     * push a color to the message, length is 3 size of chars
     * @param red
     * @param green
     * @param blue
    */
    void push(int red, int green, int blue) {
      this->push(static_cast<char>(red));
      this->push(static_cast<char>(green));
      this->push(static_cast<char>(blue));
    }
    /**
     * calculate the length, and call the sendMessage
     */
    ~MessageChannel() {
      // calculate the length of the message
      this->message_[1] = static_cast<char>((this->length >> 12) & 0xf);
      this->message_[2] = static_cast<char>((this->length >> 8) & 0xf);
      this->message_[3] = static_cast<char>((this->length >> 4) & 0xf);
      this->message_[4] = static_cast<char>(this->length & 0xf);
      client_->sendMessage(this->message_);
    }
   private:
    GraphicsClient *client_;
    int length;
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

  /**
   * draws a rectangle at the specified coordinates given by the first two parameters of the specified size given by the last two parameters.
   * @param x
   * @param y
   * @param w width
   * @param h height
   */
  void drawRectangle(int x, int y, int w, int h);

  /**
   * draws a filled rectangle at the position and size given by the parameters.
   * @param x
   * @param y
   * @param w width
   * @param h height
   */
  void fillRectangle(int x, int y, int w, int h);

  /**
   * clears (sets the pixels to the background color) at the location and size specified by the parameters.
   * @param x
   * @param y
   * @param w width
   * @param h height
   */
  void clearRectangle(int x, int y, int w, int h);

  /**
   * draws an oval at the specified location inscribed in a rectangle of the specified size.
   * @param x
   * @param y
   * @param w width
   * @param h height
   */
  void drawOval(int x, int y, int w, int h);

  /**
   * is the same as the drawOval method except the oval is filled.
   * @param x
   * @param y
   * @param w width
   * @param h height
   */
  void fillOval(int x, int y, int w, int h);

  /**
   * draws a line starting a point 1 and ending at point 2.
   * @param x1
   * @param y1
   * @param x2
   * @param y2
   */
  void drawLine(int x1, int y1, int x2, int y2);

  /**
   * draws a string of characters on the display.
   * @param x
   * @param y
   * @param str string that will display
   */
  void drawstring(int x, int y, const std::string &str);

  /**
   * send the redraw (repaint) signal to the attached graphics server.
   */
  void repaint();

  /**
   *
   * @param url
   * @param port
   * @example
   *    GraphicsClient client("localhost", 7777);
   */
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
