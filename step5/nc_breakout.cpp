#include <ncurses.h>
#include <random>
#include <future>

int px = 40;
const int py = 23;
bool now_playing = true;
bool has_ball = true;
double bx = 0.0, by = 0.0;
double vx = 0.0, vy = 0.0;
const int SX = 10, SY = 2;
const int WX = 60, WY = 5;
bool blocks[WX][WY];

void
draw_all(void) {
  clear();
  //パドルの描画
  if (has_ball) {
    mvprintw(py - 1, px, "*");
  }
  mvprintw(py, px - 2, "-----");
  //ボールの描画
  int x = static_cast<int>(bx);
  int y = static_cast<int>(by);
  if (!has_ball) {
    mvprintw(y, x, "*");
  }
  //ブロックの描画
  for (int i = 0; i < WX; i++) {
    for (int j = 0; j < WY; j++) {
      if (blocks[i][j]) {
        mvprintw(j + SY, i + SX, "+");
      }
    }
  }
  refresh();
}

void
paddle_collision_check(void) {
  if (by < 23.0) return;
  if (bx < px - 2)return;
  if (bx > px + 3)return;
  by = 23;
  double theta = M_PI * ((static_cast<double>(px) - bx + 1.5) / 8.0 + 0.25);
  vx = cos(theta) * 0.5;
  vy = -sin(theta) * 0.5;
}

void
block_collision_check(void) {
  if (bx < SX)return;
  if (bx > SX + WX)return;
  if (by < SY)return;
  if (by > SY + WY)return;
  int x = static_cast<int>(bx) - SX;
  int y = static_cast<int>(by) - SY;
  if (!blocks[x][y])return;
  blocks[x][y] = false;
  double dx = bx - x + 0.5;
  double dy = by - y + 0.5;
  if (abs(dx) < abs(dy)) {
    vx = -vx;
  } else {
    vy = -vy;
  }
}

void
move_ball(void) {
  if (has_ball)return;
  paddle_collision_check();
  block_collision_check();
  bx += vx;
  by += vy;
  if (bx < 0) {
    bx = 0;
    vx = -vx;
  }
  if (by < 0) {
    by = 0;
    vy = -vy;
  }
  if (bx > 80) {
    bx = 80;
    vx = -vx;
  }
  if (by > 24) {
    by = 24;
    has_ball = true;
  }
}

void
game_loop(void) {
  while (now_playing) {
    move_ball();
    draw_all();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

int
main(void) {
  initscr();
  noecho(); //キーが入力されても表示しない
  curs_set(0);//カーソルを非表示
  keypad(stdscr, TRUE); // xtermでマウスイベントの取得に必要
  mousemask(REPORT_MOUSE_POSITION, NULL);//マウスイベントを取得
  // ブロック初期化
  for (int i = 0; i < WX; i++) {
    for (int j = 0; j < WY; j++) {
      blocks[i][j] = true;
    }
  }
  MEVENT e;
  draw_all();
  auto th_game = std::thread([] {game_loop();});
  std::mt19937 mt;
  std::uniform_real_distribution<double> ud(0.0, 1.0);
  while (true) {
    int ch = getch();
    if (ch == 'q') break;
    if (has_ball && ch == ' ') {
      has_ball = false;
      bx = px;
      by = py - 1.0;
      double theta = (ud(mt) * 0.5 + 0.25) * M_PI;
      vx = cos(theta) * 0.5;
      vy = -sin(theta) * 0.5;
    }
    if (ch == KEY_MOUSE) {
      if (getmouse(&e) == OK) {
        px = e.x;
        if (px < 2)px = 2;
        if (px > 77)px = 77;
      }
    }
  }
  now_playing = false;
  th_game.join();
  endwin();
}
