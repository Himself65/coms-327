#include "CellularAutomaton.h"

CellularAutomaton::CellularAutomaton(std::string path,
                                     int q_state)
    : client_(nullptr), path_(std::move(path)), q_state_(q_state) {
  this->infile_ = std::ifstream(path);
  this->infile_ >> this->width_ >> this->height_;

  this->array_ = new int *[this->width_];
  for (int i = 0; i < this->width_; i++) {
    this->array_[i] = new int[this->height_];
  }
  for (int y = 0; y < this->height_; y++) {
    for (int x = 0; x < this->width_; x++) {
      int v;
      this->infile_ >> v;
      this->array_[x][y] = v;
    }
  }
}

CellularAutomaton &CellularAutomaton::operator=(const CellularAutomaton &that) {
  if (this != &that) {
    this->path_ = std::string(that.path_);
    this->q_state_ = that.q_state_;
    this->infile_.close();
    this->infile_ = std::ifstream(this->path_);
  }
  return *this;
}

CellularAutomaton::~CellularAutomaton() {
  this->infile_.close();
}

void CellularAutomaton::connectClient(GraphicsClient &client) {
  this->client_ = &client;
}

void CellularAutomaton::step(unsigned char (*callback)(CellularAutomaton &, int, int)) {
  if (this->client_ == nullptr) {
    std::cerr << "cannot find client" << std::endl;
    return;
  }
  int **new_data = new int *[this->width_];
  for (int i = 0; i < this->width_; i++) {
    new_data[i] = new int[this->height_];
  }

  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      new_data[x][y] = callback(*this, x, y);
    }
  }

  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      this->array_[x][y] = new_data[x][y];
    }
  }

  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      if (new_data[x][y] != 0) {
        this->client_->setPixel(x, y, 255, 255, 255);
      } else {
        this->client_->setPixel(x, y, 0, 0, 0);
      }
    }
  }
  this->client_->repaint();

  for (int i = 0; i < this->width_; i++) {
    delete[] new_data[i];
  }
  delete[] new_data;
}

int *CellularAutomaton::operator[](int x) {
  return this->array_[x];
}

