#include "CellularAutomaton.h"

CellularAutomaton::CellularAutomaton(const std::string &path,
                                     int q_state)
    : client_(nullptr), path_(path), q_state_(q_state) {
  this->file_ = fopen(path.c_str(), "r");
  if (this->file_ == nullptr) {
    std::cerr << "cannot open file" << std::endl;
    exit(EXIT_FAILURE);
  }
  fscanf(this->file_, "%d %d", &this->width_, &this->height_);

  this->array_ = new int *[this->height_];
  for (int i = 0; i < this->height_; i++) {
    this->array_[i] = new int[this->width_];
  }
  for (int x = 0; x < this->width_; x++) {
    for (int y = 0; y < this->height_; y++) {
      int v;
      fscanf(this->file_, "%d", &v);
      this->array_[x][y] = v;
    }
  }
}

CellularAutomaton &CellularAutomaton::operator=(const CellularAutomaton &that) {
  if (this != &that) {
    this->path_ = std::string(that.path_);
    this->q_state_ = that.q_state_;
    fclose(this->file_);
    this->file_ = fdopen(dup(fileno(that.file_)), "r"); // open that file

    fscanf(this->file_, "%d %d", &this->width_, &this->height_);
    for (int y = 0; y < this->height_; y++) {
      for (int x = 0; x < this->width_; x++) {
        int v;
        fscanf(this->file_, "%d", &v);
        this->array_[x][y] = v;
      }
    }
  }
  return *this;
}

CellularAutomaton::~CellularAutomaton() {
  if (fclose(this->file_) != 0) {
    std::cerr << "cannot close the file" << std::endl;
    exit(EXIT_FAILURE);
  }
  this->file_ = nullptr;

  for (int i = 0; i < this->width_; i++) {
    delete[] this->array_[i];
  }
  delete[] this->array_;
}

void CellularAutomaton::connectClient(GraphicsClient &client) {
  this->client_ = &client;
}

void CellularAutomaton::step(unsigned char (*callback)(CellularAutomaton &, int, int)) {
  if (this->client_ == nullptr) {
    std::cerr << "cannot find client" << std::endl;
    return;
  }

  int **new_data = new int *[this->height()];
  for (int i = 0; i < this->height(); i++) {
    new_data[i] = new int[this->width()];
  }

  // simulate the next step
  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      new_data[x][y] = callback(*this, x, y);
    }
  }

  // save the new data
  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      this->array_[x][y] = new_data[x][y];
    }
  }

  // send the data to the server
  this->client_->clear();
  const int size = this->size();
  const int gap = this->gap();
  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      if (this->array_[x][y] != 0) {
        this->client_->fillRectangle(x * size + gap * size, y * size + gap * size, size, size);
      }
    }
  }
  this->client_->repaint();

  // free the new_data
  for (int i = 0; i < this->width_; i++) {
    delete[] new_data[i];
  }
  delete[] new_data;
}

int *CellularAutomaton::operator[](int x) {
  return this->array_[x];
}

