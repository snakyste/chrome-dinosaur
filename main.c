// build with "gcc main.c game.c menu.c appearance.c -lncurses -o dino.exe"

/**
	\file main.c
	\brief Gallo Alessandro ha effettuato i commenti secondo tag doxygen di questo file
*/

#include <ncurses.h>
#include <stdio.h>
#include "menu.h"
#include "minunit.h"

/**
	\fn int main(int argc, char **argv)
	\brief funzione principale
	\param int argc: contiene il numero di argomenti che vengono inseriti via riga di comando
	\param char **argv: contiene gli argomenti che vengono inseriti via riga di codice
	\return ritorna il valore di 0
*/

int main(int argc, char **argv) {
	// Run tests (if the -test command argument has been specified)
	//int testsResult = run_all_tests(argc, argv);
	//if(testsResult > 0)
	//	return testsResult != 0;

	// This is the normal start
	initscr();
	start_color();
	curs_set(FALSE);
	startMenu();
	endwin();

    return 0;
}
