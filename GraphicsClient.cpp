#include "GraphicsClient.h"

void GraphicsClient::connectServer() {
  struct sockaddr_in server_address = {};
  int fd;
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "socket create error" << std::endl;
  }
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_);
  if (inet_pton(AF_INET, this->url_.data(), &server_address.sin_addr) < 0) {
    std::cerr << "invalid address" << std::endl;
  }
  // connect to the server using address
  if (connect(fd, (struct sockaddr *) &server_address, sizeof(server_address))) {
    std::cerr << "connection failed";
  }
  this->server_fd_ = fd;
}

void GraphicsClient::sendMessage(std::vector<char> message) const {
  unsigned long expected = message.size();
  ssize_t actual;
  if ((actual = send(this->server_fd_, message.data(), expected, 0)) != expected) {
    std::cerr << "incorrect send message_, expected " << expected << ", actual " << actual << std::endl;
  }
}

void GraphicsClient::clear() {
  MessageChannel channel(this, Mnemonic::CLEAR);
}

void GraphicsClient::repaint() {
  MessageChannel channel(this, Mnemonic::REPAINT);
}

void GraphicsClient::setDrawingColor(int r, int g, int b) {
  MessageChannel channel(this, Mnemonic::SET_DRAWING_COLOR);
  channel.push(r, g, b);
}

void GraphicsClient::setBackgroundColor(int r, int g, int b) {
  MessageChannel channel(this, Mnemonic::SET_BACKGROUND_COLOR);
  channel.push(r, g, b);
}

void GraphicsClient::setPixel(int x, int y, int r, int g, int b) {
  MessageChannel channel(this, Mnemonic::SET_PIXEL);
  channel.push(x);
  channel.push(y);
  channel.push(r, g, b);
}

void GraphicsClient::drawstring(int x, int y, const std::string &str) {
  MessageChannel channel(this, Mnemonic::DRAW_STRING);
  channel.push(x);
  channel.push(y);
  channel.push(str);
}

void GraphicsClient::drawRectangle(int x, int y, int w, int h) {
  MessageChannel channel(this, Mnemonic::DRAW_RECTANGLE);
  channel.push(x);
  channel.push(y);
  channel.push(w);
  channel.push(h);
}

void GraphicsClient::fillRectangle(int x, int y, int w, int h) {
  MessageChannel channel(this, Mnemonic::FILL_RECTANGLE);
  channel.push(x);
  channel.push(y);
  channel.push(w);
  channel.push(h);
}

void GraphicsClient::clearRectangle(int x, int y, int w, int h) {
  MessageChannel channel(this, Mnemonic::CLEAR_RECTANGLE);
  channel.push(x);
  channel.push(y);
  channel.push(w);
  channel.push(h);
}

void GraphicsClient::drawOval(int x, int y, int w, int h) {
  MessageChannel channel(this, Mnemonic::DRAW_OVAL);
  channel.push(x);
  channel.push(y);
  channel.push(w);
  channel.push(h);
}

void GraphicsClient::fillOval(int x, int y, int w, int h) {
  MessageChannel channel(this, Mnemonic::FILL_OVAL);
  channel.push(x);
  channel.push(y);
  channel.push(w);
  channel.push(h);
}

void GraphicsClient::drawLine(int x1, int y1, int x2, int y2) {
  MessageChannel channel(this, Mnemonic::DRAW_LINE);
  channel.push(x1);
  channel.push(y1);
  channel.push(x2);
  channel.push(y2);
}

void GraphicsClient::fileRequest() {
  MessageChannel channel(this, Mnemonic::FILE_REQUEST);
}

std::vector<unsigned char> *GraphicsClient::handle_response() const {
  int count;
  ioctl(this->server_fd_, FIONREAD, &count);
  if (count > 0) {
    auto* response = new std::vector<unsigned char>(count);
    if (count != read(this->server_fd_, response->data(), count)) {
      std::cerr << "incorrect response data size" << std::endl;
    }
    return response;
  } else {
    return new std::vector<unsigned char>();
  }
}

GraphicsClient::GraphicsClient(std::string url, int port) : url_(std::move(url)), port_(port), server_fd_(0) {
  connectServer();
}

GraphicsClient::GraphicsClient(GraphicsClient &that) : url_(std::string(that.url_)), port_(that.port_), server_fd_(0) {
  connectServer();
}

void GraphicsClient::disconnectServer() {
  if (close(this->server_fd_) != 0) {
    std::cerr << "close server fd error" << std::endl;
  } else {
    this->server_fd_ = 0;
  }
}

GraphicsClient::~GraphicsClient() {
  this->disconnectServer();
}

GraphicsClient &GraphicsClient::operator=(const GraphicsClient &that) {
  if (this != &that) {
    this->disconnectServer();
    this->port_ = that.port_;
    this->url_ = std::string(that.url_);
    this->connectServer();  // connect to the new port_
  }
  return *this;
}
