#include <format>
#include <memory>
#include <ncurses.h>
#include <stdexcept>
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
  Component(int _x, int _y) : x(_x), y(_y) {}
  virtual ~Component() {}

  virtual void _render() = 0;
  virtual bool hitTest(int _x, int _y) { return (_x == x) && (_y == y); }

protected:
  int x, y;
};

class Empty : public Component {
public:
  Empty(int _x, int _y) : Component(_x, _y) {}
  virtual void _render() {}
  virtual bool hitTest(int x, int y) { return Component::hitTest(x, y); }
};

class Tee : public Component {
public:
  Tee(int _x, int _y) : Component(_x, _y) {}
  ~Tee() {}

  virtual void _render() {
    switch (orientation) {
    case UP:
      mvaddch(y, x, ACS_BTEE);
      return;
    case RIGHT:
      mvaddch(y, x, ACS_LTEE);
      return;
    case DOWN:
      mvaddch(y, x, ACS_TTEE);
      return;
    case LEFT:
      mvaddch(y, x, ACS_RTEE);
      return;
    }
  }

  virtual bool hitTest(int x, int y) {
    if (Component::hitTest(x, y)) {
      orientation = static_cast<Orientation>((orientation + 1) % 4);
      return true;
    } else {
      return false;
    }
  }

  Orientation orientation = UP;
};

// class Terminal : public Component {};

class State {
public:
  State() {
    initscr();
    keypad(stdscr, true);
    noecho(); // no echo from mouse input
    if (NCURSES_MOUSE_VERSION <= 0) {
      throw std::runtime_error("NCURSES does not have mouse support");
    }
    mousemask(BUTTON1_PRESSED, nullptr);

    for (int x = 0; x < 10; x++) {
      for (int y = 0; y < 10; y++) {
        //if ((x + y) % 2 == 0) {
          grid.push_back(std::make_unique<Tee>(x, y));
        //} else {
        //  grid.push_back(std::make_unique<Empty>(x, y));
        //}
      }
    }
  }
  ~State() { endwin(); }

  std::vector<std::unique_ptr<Component>> grid;

  void _renderGrid() {
    for (auto &comp : grid) {
      comp->_render();
    }
  }

  void _hitTest(int x, int y) {
    for (auto &comp : grid) {
      if (comp->hitTest(x, y)) {
        return;
      }
    }
  }

  State(const State &) = delete;
  State(State &&) = delete;
  State &operator=(const State &) = delete;
  State &operator=(State &&) = delete;
};

} // namespace NNetwalk

int main() {
  using namespace NNetwalk;

  MEVENT event;
  auto state = State();

  while (1) {
    clear();

    state._renderGrid();
    auto c = getch();
    if (c == KEY_MOUSE) {
      getmouse(&event);
      mvprintw(12, 2, "event (%d, %d)", event.x, event.y);
      state._hitTest(event.x, event.y);
    } else {
      mvprintw(12, 2, "Error 0x%02X %d %c", c, c, c);
    }

    refresh();
  }

  return 0;
}
