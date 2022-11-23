// build with "gcc main.c game.c menu.c appearance.c -lncurses -o dino.exe"

#include <ncurses.h>
#include <stdio.h>
#include "menu.h"
#include "minunit.h"

int main(int argc, char **argv) {
	// Run tests (if the -test command argument has been specified)
	int testsResult = run_all_tests(argc, argv);
	if(testsResult > 0)
		return testsResult != 0;

	// Normal start
	initscr();
	start_color();
	curs_set(FALSE);
	startMenu();
	endwin();

    return 0;
}
