#ifndef KREATIVITAS_H
#define KREATIVITAS_H

#include <stdio.h>
#include <unistd.h>

#define GREEN "\033[32m"
#define RESET "\033[0m"

// help
void printMenu();

// ascii art
void printBanner();

// loading
void loadingBar(int length, int duration);
void loadingBarSmooth(int length, int delayMicroseconds);

// command
// void 

#endif