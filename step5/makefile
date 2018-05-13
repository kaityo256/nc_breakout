all: a.out
LDFLAGS=-lncurses

a.out: nc_breakout.cpp
	g++ -O3 $(LDFLAGS) $<

run: a.out
	TERM=xterm-1003 ./a.out

clean:
	rm -f a.out


