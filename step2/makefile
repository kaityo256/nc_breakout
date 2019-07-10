all: a.out
LDFLAGS=-lncurses

a.out: test.cpp
	g++ $< $(LDFLAGS)

clean:
	rm -f a.out


