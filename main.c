// build with "gcc main.c game.c menu.c -lncurses -o dino.exe"

#include <ncurses.h>
#include "menu.h"

int main() {
	initscr();
	start_color();
	curs_set(FALSE);
	startMenu();
	endwin();
}
