#ifndef MENU_H
#define MENU_H

struct user {
	char name[20];
	char lastName[20];
	char age[3];
};

void startMenu();
void endGame(int score, int highScore, int diY, int diX, struct user firstUser);

#endif