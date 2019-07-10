all: a.out
CPPFLAGS=-O3 -std=c++11
LDFLAGS=-lncurses -lpthread

a.out: nc_breakout.cpp
	g++ $(CPPFLAGS) $< $(LDFLAGS)

run: a.out
	TERM=xterm-1003 ./a.out

clean:
	rm -f a.out


