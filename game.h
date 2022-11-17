#ifndef GAME_H
#define GAME_H

void startEngine(int highScore, struct user firstUser);
int computePrize(int score, int usedPrize);
void showdinosaur(int diY, int diX);
int computeTime(int delayTime);
int checkGame(int y, int x, int diY, int diX);

#endif