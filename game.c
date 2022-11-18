#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "menu.h"
#include "game.h"
#include "appearance.h"

// Check if the game is going to be finished in this turn of loop
int checkGame(int y, int x, int diY, int diX) {
	if (diY == y) {
		if (abs((diX+14)-x) <= 4) {
			return 0;
		}
	}
	return 1;
}

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


// Give user the arrow
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
    // When code reaches here, means that user is loss
	endGame(score, highScore, diY, diX, firstUser);
	attroff(COLOR_PAIR(1));
}
