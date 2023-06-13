#include <ncurses.h>

int main(void) {
  initscr();
  noecho();    // キーが入力されても表示しない
  curs_set(0); // カーソルを非表示
  mvprintw(12, 30, "Hello World!");
  while (true) {
    int ch = getch();
    if (ch == 'q') break;
  }
  endwin();
}
