#include "CellularAutomaton.h"

static unsigned char aRule(CellularAutomaton &ca, int x, int y) {
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
  } else if (count < 2) {
    exit = 0;
  } else if (count == 2 || count == 3) {
    exit = 1;
  } else if (count > 3) {
    exit = 0;
  }

  //  printf("pos(%d, %d) %d from %d~> %d\n", x, y, count, ca[x][y], exit);
  return exit;
}

CellularAutomaton::CellularAutomaton()
    : client_(nullptr), file_(nullptr),
      mode_(Mode::Pause), type_(Large),
      width_(0), height_(0) {
  srand(time(NULL));
}

void CellularAutomaton::loadFile(const std::string &path) {
  this->file_ = fopen(path.data(), "r");
  if (this->file_ == nullptr) {
    std::cerr << "cannot open file" << std::endl;
    exit(EXIT_FAILURE);
  }
  fscanf(this->file_, "%d %d", &this->width_, &this->height_);

  if (this->array_ != nullptr) {
    // remove the old array
    for (int i = 0; i < this->height(); i++) {
      delete[] this->array_[i];
    }
    delete[] this->array_;
  }

  this->init_array_ = new int *[this->height()];
  this->array_ = new int *[this->height()];
  for (int i = 0; i < this->height(); i++) {
    this->init_array_[i] = new int[this->width()];
    this->array_[i] = new int[this->width()];
  }
  for (int x = 0; x < this->width(); x++) {
    for (int y = 0; y < this->height(); y++) {
      int v;
      fscanf(this->file_, "%d", &v);
      this->array_[x][y] = v;
      this->init_array_[x][y] = v;
    }
  }
  this->print();
}

CellularAutomaton &CellularAutomaton::operator=(const CellularAutomaton &that) {
  if (this != &that) {
    this->path_ = std::string(that.path_);
    if (fclose(this->file_) != 0) {
      std::cerr << "cannot close the file" << std::endl;
      exit(EXIT_FAILURE);
    }
    this->file_ = fdopen(dup(fileno(that.file_)), "r"); // open that file

    fscanf(this->file_, "%d %d", &this->width_, &this->height_);
    for (int i = 0; i < this->height(); i++) {
      delete[] this->array_[i];
      delete[] this->init_array_[i];
    }
    delete[] this->array_;
    delete[] this->init_array_;

    this->array_ = new int *[this->height()];
    this->init_array_ = new int *[this->height()];
    for (int i = 0; i < this->height(); i++) {
      this->array_[i] = new int[this->width()];
      this->init_array_[i] = new int[this->width()];
    }
    for (int y = 0; y < this->height(); y++) {
      for (int x = 0; x < this->width(); x++) {
        int v;
        fscanf(this->file_, "%d", &v);
        this->array_[x][y] = v;
        this->init_array_[x][y] = v;
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

  for (int i = 0; i < this->height(); i++) {
    delete[] this->init_array_[i];
    delete[] this->array_[i];
  }
  delete[] this->array_;
  delete[] this->init_array_;
}

void CellularAutomaton::connectClient(GraphicsClient &client) {
  this->client_ = &client;
  ButtonScope(this, {650, 100, 50, 30}, "STEP");
  ButtonScope(this, {650, 150, 50, 30}, "RUN");
  ButtonScope(this, {650, 200, 50, 30}, "PAUSE");
  ButtonScope(this, {650, 250, 50, 30}, "RESET");
  ButtonScope(this, {650, 300, 70, 30}, "RANDOM");
  ButtonScope(this, {650, 350, 50, 30}, "LOAD");
  ButtonScope(this, {650, 400, 50, 30}, "QUIT");
  ButtonScope(this, {630, 450, 30, 30}, "1");
  ButtonScope(this, {670, 450, 30, 30}, "2");
  ButtonScope(this, {710, 450, 30, 30}, "3");
  this->client_->drawRectangle(0, 0, 600, 600);
  client.repaint();
  std::cout << "paint button success!" << std::endl;
}

int *CellularAutomaton::operator[](int x) {
  return this->array_[x];
}

inline int parse_number(unsigned char high, unsigned char high_low, unsigned char low_high, unsigned char low) {
  return (high << 12) +
      (high_low << 8) +
      (low_high << 4) +
      (low & 0xf);
}

void CellularAutomaton::simulate() {
  timespec timespec = {
      0, 100000000L // 100ms
  };
  while (true) {
    auto *response = client_->handle_response();
    if (!response->empty()) {
      auto index = 0;
      while (index < response->size()) {
        if (response->at(index++) != 0xFF) {
          std::cerr << "sync error" << std::endl;
        }
        const auto length = parse_number(response->at(index),
                                         response->at(index + 1),
                                         response->at(index + 2),
                                         response->at(index + 3));
        index += 4;
        if (response->at(index) >= 0x01 && response->at(index) <= 0x03) {
          if (length != 10) {
            std::cerr << "length error" << std::endl;
          }
          index++;
          const auto button_number =
              static_cast<unsigned char>(response->at(index++));  // 0, 1, 2. Means left, middle, and right button.
          if (button_number != 1) {
            // we only allow left click now
            break;
          }
          int x = parse_number(response->at(index),
                               response->at(index + 1),
                               response->at(index + 2),
                               response->at(index + 3));
          int y = parse_number(response->at(index + 4),
                               response->at(index + 5),
                               response->at(index + 6),
                               response->at(index + 7));
          index += 8;
          std::cout << "you clicked: " << x << ", " << y << std::endl;

          for (const auto &pair: this->buttons_) {
            const auto &pos = pair.first;
            const auto &name = pair.second;
            if (pos.x <= x && x <= pos.x + pos.width) {
              if (pos.y <= y && y <= pos.y + pos.height) {
                // match the click area
                if (strcmp(name.data(), "STEP") == 0) {
                  this->step();
                } else if (strcmp(name.data(), "RUN") == 0) {
                  this->run();
                } else if (strcmp(name.data(), "RESET") == 0) {
                  this->reset();
                } else if (strcmp(name.data(), "RANDOM") == 0) {
                  this->random();
                } else if (strcmp(name.data(), "PAUSE") == 0) {
                  this->pause();
                } else if (strcmp(name.data(), "LOAD") == 0) {
                  this->load();
                } else if (strcmp(name.data(), "QUIT") == 0) {
                  this->quit();
                } else if (strcmp(name.data(), "1") == 0) {
                  this->select_size(1);
                } else if (strcmp(name.data(), "2") == 0) {
                  this->select_size(2);
                } else if (strcmp(name.data(), "3") == 0) {
                  this->select_size(3);
                }
              }
            }
          }
        } else {
          switch (response->at(index++)) {
            case 0x0A: {
              std::string path;
              for (auto i = index;
                   i < response->size();
                   index += 2, i += 2) {
                const auto h = response->at(i);
                const auto l = response->at(i + 1);
                const auto ch = static_cast<unsigned char>((h << 4) + l);
                path.push_back(static_cast<char>(ch));
              }
              std::cout << path << std::endl;
              this->loadFile(path);
              break;
            }
            default: {
              std::cerr << "incorrect type" << std::endl;
              return;
            }
          }
        }
      }
    }

    // check the state
    switch (this->mode_) {
      case Mode::Running: {
        this->step();
        break;
      }
      case Mode::Pause:break;
      case Mode::Quit: {
        return;
      }
    }

    delete response;  // clean up the temp data
    this->client_->repaint();
    nanosleep(&timespec, nullptr);
  }
}

void CellularAutomaton::step() {
  std::cout << "step" << std::endl;
  int **new_data = new int *[this->height()];
  for (int i = 0; i < this->height(); i++) {
    new_data[i] = new int[this->width()];
  }

  // simulate the next step
  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      new_data[x][y] = aRule(*this, x, y);
    }
  }

  // save the new data
  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      this->array_[x][y] = new_data[x][y];
    }
  }

  // print the new data
  this->print();

  // free the new_data
  for (int i = 0; i < this->height(); i++) {
    delete[] new_data[i];
  }
  delete[] new_data;
}

void CellularAutomaton::run() {
  std::cout << "run" << std::endl;
  this->mode_ = Mode::Running;
}

void CellularAutomaton::random() {
  std::cout << "random" << std::endl;
  for (int y = 0; y < this->height(); y++) {
    for (int x = 0; x < this->width(); x++) {
      this->array_[x][y] = rand() % 2;
    }
  }
}

void CellularAutomaton::reset() {
  std::cout << "reset" << std::endl;
  for (int y = 0; y < this->height(); y++) {
    for (int x = 0; x < this->width(); x++) {
      this->array_[x][y] = this->init_array_[x][y];
    }
  }
  this->print();
}

void CellularAutomaton::pause() {
  std::cout << "pause" << std::endl;
  this->mode_ = Mode::Pause;
}

void CellularAutomaton::load() {
  std::cout << "load" << std::endl;
  this->client_->fileRequest();
};

void CellularAutomaton::quit() {
  std::cout << "quit" << std::endl;
  this->mode_ = Mode::Quit;
}

void CellularAutomaton::select_size(int size) {
  std::cout << "select size " << size << std::endl;
  if (size == 1) {
    this->type_ = Large;
  } else if (size == 2) {
    this->type_ = Middle;
  } else {
    this->type_ = Small;
  }
  this->print();
}

void CellularAutomaton::print() {
  this->client_->clearRectangle(0, 0, 600, 600);
  const int size = this->size();
  const int gap = this->gap();
  for (int x = 0; x < this->width(); ++x) {
    for (int y = 0; y < this->height(); ++y) {
      if (this->array_[x][y] != 0) {
        this->client_->fillRectangle(x * size + gap * size, y * size + gap * size, size, size);
      }
    }
  }
}
