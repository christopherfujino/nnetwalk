#include <memory>
#include <ncurses.h>
#include <vector>

namespace NNetwalk {

enum Orientation {
  UP = 0,
  RIGHT = 1,
  DOWN = 2,
  LEFT = 3,
};

class Component {
public:
  virtual void _render() = 0;
};

class Empty : public Component {
public:
  Empty() {}
  virtual void _render() {}
};

class Pipe : public Component {
public:
  Pipe(int _x, int _y) : x(_x), y(_y) {}
  virtual void _render() { mvaddch(y, x, ACS_LTEE); }

protected:
  int x, y;
};

// class Terminal : public Component {};

class State {
public:
  State() {
    initscr();

    for (int x = 0; x < 10; x++) {
      for (int y = 0; y < 10; y++) {
        if ((x + y) % 2 == 0) {
          grid.push_back(std::make_unique<Pipe>(x, y));
        } else {
          grid.push_back(std::make_unique<Empty>());
        }
      }
    }
  }
  ~State() { endwin(); }

  std::vector<std::unique_ptr<Component>> grid;

  void _renderGrid() {
    clear();

    for (auto &comp : grid) {
      comp->_render();
    }

    refresh();
  }

  State(const State &) = delete;
  State(State &&) = delete;
  State &operator=(const State &) = delete;
  State &operator=(State &&) = delete;
};

} // namespace NNetwalk

int main() {
  using namespace NNetwalk;

  auto state = State();
  state._renderGrid();
  getch();

  return 0;
}
