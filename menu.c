/**
	\file menu.c
	\brief Gallo Alessandro ha effettuato i commenti secondo tag doxygen di questo file
*/

#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "game.h"
#include "appearance.h"

/**
	\fn void startMenu()
	\brief funzione che serve a stampare a video e a chiedere all'utente i campi del menu iniziale con le relative texture
*/

void startMenu() {
	struct user firstUser;
	int highScore;
    // Read high score
	FILE *highScoreFile;
	highScoreFile = fopen("./highScore.txt", "r");
	fscanf(highScoreFile, "%d", &highScore);
	fclose(highScoreFile);
	int maxX = getmaxx(stdscr)/2;
	int maxY = getmaxy(stdscr)/2;
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	attron(COLOR_PAIR(3));
	showTrex(maxY, maxX);
	attroff(COLOR_PAIR(3));
    // Get info
	mvprintw(maxY+1, maxX-28, "Write inputs and press Enter to start the Game.");
    mvprintw(maxY+2, maxX-26, "When you had prize, fire it with 'k' key!");
	mvprintw(maxY+3, maxX-21, "You can jump with space key!");
	mvprintw(maxY+4, maxX-15, "Name: ");
	getstr(firstUser.name);
	mvprintw(maxY+5, maxX-15, "Last name: ");
	getstr(firstUser.lastName);
	mvprintw(maxY+6, maxX-15, "Age: ");
	getstr(firstUser.age);
	noecho();
	startEngine(highScore, firstUser);
}

/**
	\fn void endGame(int score, int highScore, int diY, int diX, struct user firstUser)
	\brief funzione che serve per definire cio' che il programma deve fare una volta che il gioco e' finito
	\param int score: valore il cui contiene il punteggio della partita appena terminata
	\param int highScore: valore il cui contiene il punteggio massimo effettuato fino all'ultima partita effettuata
	\param int diX: prima coordinata
	\param int diY: seconda coordinata
	\param struct user firstUser: struttura che fa riferimento ai campi contenuti in menu.h
*/

void endGame(int score, int highScore, int diY, int diX, struct user firstUser) {
	nodelay(stdscr, FALSE);
	init_pair(2,COLOR_RED,COLOR_BLACK);
    // Save
	if (score > highScore) {
		highScore = score;
		FILE *highScoreFile;
		highScoreFile = fopen("./highScore.txt", "w");
		fprintf(highScoreFile, "%d", highScore);
		fclose(highScoreFile);
	}
	int maxX = getmaxx(stdscr)/2;
	int maxY = getmaxy(stdscr)/2;
	attron(COLOR_PAIR(2));
	showLoss(maxY, maxX);
	mvprintw(diY-4, diX, "          X-X ");
	mvprintw(diY, diX, "      ||");
	char keyToExit = getch();
    // Exit or restart
	if (keyToExit == 'r') {
		attroff(COLOR_PAIR(2));
		startEngine(highScore, firstUser);
	}
	else if (keyToExit == 'q') {
		return;
	}
	else {
		endGame(score, highScore, diY, diX, firstUser);
	}
}
