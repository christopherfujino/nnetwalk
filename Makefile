CXX = clang++
CFLAGS = -g -std=c++20 -Wall -Wextra -Werror -Wpedantic

.PHONY: run

run: a.out
	./a.out

a.out: main.cc
	$(CXX) $(CFLAGS) $< -lncurses -o $@
