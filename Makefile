CXX = clang++
CFLAGS = -g

.PHONY: run

run: a.out
	./a.out

a.out: main.cc
	$(CXX) $(CFLAGS) $< -lncurses -o $@
