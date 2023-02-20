/**
	\file game.c
	\brief Documentazione secondo tag doxygen effettuata da Alessandro Gallo
*/

#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "game.h"
#include "appearance.h"
#include "minunit.h"

/**
	\fn int checkGame(int y, int x, int diY, int diX)
	\brief funzione che definisce se il gioco dev'essere terminato
	\param int y: coordinata 1
	\param int x: coordinata 2
	\param int diY: coordinata 3
	\param int diX: coordinata 4
	\return la funzione restituisce il valore 0 in caso il gioco sia finito e 1 in caso debba continuare il loop
*/

// Check if the game is going to be finished in this turn of loop
int checkGame(int y, int x, int diY, int diX) {
	if (diY == y) {
		if (abs((diX+14)-x) <= 4) {
			return 0;
		}
	}
	return 1;
}

/**
	\fn int computeTime(int delayTime)
	\brief funzione che definisce l'aumento della velocita' del movimento del path
	\param int delayTime: variabile al cui interno definisce il delay applicato nel momento in cui viene chiamata ka funzione
	\return ritorna il valore del delay che verra' variato
*/

// Make game faster
int computeTime(int delayTime) {
	if (delayTime >= 250000) {
		delayTime -= 1000;
	}
	else if (delayTime >= 200000) {
		delayTime -= 600;
	}
	else {
		delayTime -= 200;
	}
	return delayTime;
}

/**
	\fn void showdinosaur(int diY, int diX)
	\brief funzione che serve a definire la texture del dinosauro
	\param int diY: coordinata 1
	\param int diX: coordinata 2
*/

// Which dinosaur should be printed
void showdinosaur(int diY, int diX) {
	static int counter = 0;
	if (counter == 0) {
		dinosaur1(diY, diX);
		counter++;
	}
	else {
		dinosaur2(diY, diX);
		counter--;
	}
}

/**
	\fn int computePrize(int score, int usedPrize)
	\brief funzione che serve ad assegnare i prize da utilizzare in gioco
	\param int score: punteggio dell'utente
	\param int usedPrize: prize utilizzati
*/

// Give user the prize
int computePrize(int score, int usedPrize) {
	if (score >= 20 && score <= 40 && usedPrize == 0) {
		return 1;
	}
	else if (score >= 60 && score <= 80 && usedPrize <= 1) {
		return 1;
	}
	else if (score >= 100 && score <= 120 && usedPrize <= 2) {
		return 1;
	}	
	return 0;
}

/**
	\brief in questa funzione riceve in ingresso due parametri e stampa a video le principali 
	\param int highscore: punteggio massimo effettuato fino ad all'ora al gioco
	\param struct user firstUser: sarebbero i campi con i dati dell'utente contenuti nella struttura in menu.h
*/

// The main engine!
void startEngine(int highScore, struct user firstUser) {
    srand(time(NULL));
	int x, y, diX=5, prize=0, usedPrize=0, score=0, delayTime = 300000
    , gameStatus=1, cactusNum=0;
	int maxX=getmaxx(stdscr);
	x = maxX-20;
	y = getmaxy(stdscr)-6;
	int diY = y; 
	int arrowX=(diX+15), arrowY=(diY-3);
	char userInput;
	int jumping=-1;
    bool fire=FALSE;
    clear();
	nodelay(stdscr, TRUE);
  	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(4,COLOR_BLUE,COLOR_BLACK);
	init_pair(5,COLOR_GREEN,COLOR_BLACK);
	init_pair(6,COLOR_YELLOW,COLOR_BLACK);
	while (gameStatus == 1) {
		userInput = getch();
		// Show day or night
		if((score/50)%2 != 0) {
			moon(10, (maxX/2)-10);
		}
		else {
			attron(COLOR_PAIR(1));
			sun(10, (maxX/2)-10);			
		}
		// clear arrow
		if (fire) {
			mvprintw(arrowY, arrowX-2, " ");
		}

		score++;
        // Show informations
		mvprintw(1, 6, "%s %s %s", firstUser.name, firstUser.lastName, firstUser.age);
		mvprintw(1, getmaxx(stdscr)-9, "%d:%d", highScore, score);
		// Use prize to destroy cactus
		prize = computePrize(score, usedPrize);
		mvprintw(3, 6, "Prize: %d    ", prize);
		if (prize == 1) {
			if (userInput == 'k') {
				usedPrize++;
				fire = TRUE;
			}
		}
		if (fire) {
			mvprintw(arrowY, arrowX, "*");
			arrowX += 2;
		}
		if ((x+4)-arrowX <= 1 && fire) {
			clearCactus1(y, x-1);
			mvprintw(arrowY, arrowX-2, " ");
			x = getmaxx(stdscr)-20;
			fire = FALSE;
			arrowX = diX+15;
		}
		// ----------
		box(stdscr, ACS_VLINE, ACS_HLINE);
		//for clearing screen
		cleardinosaurUp(diY, diX);
		if (x <= 7) {
			x = getmaxx(stdscr)-20;
            cactusNum = rand() % 2;
		}
        // if input is equal to ' ' then jump
		if (userInput == ' ' && jumping<0) {
			diY -= 7;
			jumping = 3;
		}
		showdinosaur(diY, diX);
		if (userInput == ' ') {
			cleardinosaurDown(diY, diX);		
		}
		if (x-diX <= 7) {
			x -= 10;
		}
        if (cactusNum == 0) {
        	cactus1(y, x);
		}
		else {
			cactus2(y, x);
		}
		if (x-diX <= 7) {
			x += 10;
		}
		gameStatus = checkGame(y, x, diY, diX);
        // Bring back dinosaur
		jumping--;
		if (jumping==0) {
			diY += 7;
		}
		mvhline(y+1, 1, '-', getmaxx(stdscr)-3);
        refresh();
        clearCactus1(y, x);
		refresh();
        usleep(delayTime);
        x -= 7;
        delayTime = computeTime(delayTime);
		userInput = 'q';
	}

	endGame(score, highScore, diY, diX, firstUser);
	attroff(COLOR_PAIR(1));
}


//
// START of tests section
//

// sample variables to be tested
int foo = 4;
int bar = 5;

// init tests vars
int tests_run = 0;
int tests_passed = 0;

// sample test function 1
static char * test_foo() {
    mu_assert("error, foo != 7", foo == 7);
}

// sample test function 2
static char * test_bar() {
    mu_assert("error, bar != 5", bar == 5);
}

/**
	\fn void all_tests()
	\brief funzione che serve per i test
*/

// test  computePrize
void test_fake()
{
	mu_assert("Errore, >=20 <=40", computePrize(20,40)==0);
}

void test_fake2()
{
	mu_assert("Errore, >=60 <=80", computePrize(60,80)<=1);
}

void test_fake3()
{
	mu_assert("Errore, >=100 <=120", computePrize(100,120)<=2);
}

void test_fake4()
{
	mu_assert("Errore, >=140 <=160", computePrize(140,160)<=3);
}

void test_fake5()
{
	mu_assert("Errore, >=180 <=200", computePrize(180,200)<=4);
}


void test_vero()
{
	mu_assert("Giusto, >= 20 <=40", computePrize(20,40)==0);
}
void test_vero2()
{
	mu_assert("Giusto, >=60 <=80", computePrize(60,80)<=1);
}

void test_vero3()
{
	mu_assert("Giusto, >=100 <=120", computePrize(100,120)<=2);
}

void test_vero4()
{
	mu_assert("Giusto, >=140 <=160", computePrize(140,160)<=3);
}

void test_vero5()
{
	mu_assert("Giusto, >=180 <=200", computePrize(180,200)<=4);
}

void test_checkgame_1()
{
	mu_assert("Corretto test 1 funzione checkGame", checkGame(4,0,5,0)==1);
}

void test_checkGame_2()
{
	mu_assert("Errore test 2 checkGame", checkGame(4,0,5,0)==0);
}

void test_checkGame_3()
{
	mu_assert("Corretto test 3 checkGame", checkGame(4,0,4,0)==1);
}

void test_checkGame_4()
{
	mu_assert("Corretto test 4 checkGame", checkGame(4,2,4,1)==1);
}

void test_checkGame_5()
{
	mu_assert("Corretto test 5 checkGame", checkGame(4,-5,4,1)==1);
}

void test_checkGame_6()
{
	mu_assert("Errore test 6 checkGame", checkGame(4,-5,4,1)==1);
}

void test_checkGame_7()
{
	mu_assert("Corretto test 7 checkGame", checkGame(4,20,4,1)==0);
}

void test_checkGame_8()
{
	mu_assert("Errore test 8 checkGame", checkGame(4,20,4,1)==1);
}

void test_checkGame_9()
{
	mu_assert("Corretto test 9 checkGame", checkGame(4,-20,4,1)==1);
}

void test_checkGame_10()
{
	mu_assert("Errore test 10 checkGame", checkGame(4,-20,4,1)==0);
}

// put all tests here
void all_tests() {
	mu_run_test(test_foo);
	mu_run_test(test_bar);
	mu_run_test(test_fake);
	mu_run_test(test_fake2);
	mu_run_test(test_fake3);
	mu_run_test(test_fake4);
	mu_run_test(test_vero);
	mu_run_test(test_vero2);
	mu_run_test(test_vero3);

	//test checkGame

	mu_run_test(test_checkgame_1);
	mu_run_test(test_checkgame_2);
	mu_run_test(test_checkgame_3);
	mu_run_test(test_checkgame_4);
	mu_run_test(test_checkgame_5);
	mu_run_test(test_checkgame_6);
	mu_run_test(test_checkgame_7);
	mu_run_test(test_checkgame_8);
	mu_run_test(test_checkgame_9);
	mu_run_test(test_checkgame_10);
	
}

/**
	\fn int run_all_tests(int argc, char **argv)
	\brief funzione che serve per far effettivamente girare i test
	\param int arcg: numero argomenti forniti da linea di comando
	\param int **argv: argomenti che vengono forniti via riga di comando
	\return fa ritornare come valore -1
*/

// call this to run all tests
int run_all_tests(int argc, char **argv) {
	if(argc<2 || strcmp(argv[1],"-test")!=0) {
		return -1;
	}
	
	printf("--- RUNNING TESTS ---\n");
    all_tests();
	printf("--- SUMMARY ---\n");
    printf("Total number of tests: %d\n", tests_run);
	printf("Tests passed: %d\n", tests_passed);
	return tests_run-tests_passed;
}
//
// END of tests section
//
