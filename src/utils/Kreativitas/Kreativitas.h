#ifndef KREATIVITAS_H
#define KREATIVITAS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define RESET           "\033[0m"
#define BOLD            "\033[1m"
#define DIM             "\033[2m"

#define BLACK           "\033[30m"
#define RED             "\033[31m"
#define GREEN           "\033[32m"
#define YELLOW          "\033[33m"
#define BLUE            "\033[34m"
#define MAGENTA         "\033[35m"
#define CYAN            "\033[36m"
#define WHITE           "\033[37m"

#define BOLD_RED        "\033[1;31m"
#define BOLD_GREEN      "\033[1;32m"
#define BOLD_YELLOW     "\033[1;33m"
#define BOLD_BLUE       "\033[1;34m"
#define BOLD_MAGENTA    "\033[1;35m"
#define BOLD_CYAN       "\033[1;36m"
#define BOLD_WHITE      "\033[1;37m"

//background
#define BG_BLACK        "\033[40m"
#define BG_RED          "\033[41m"
#define BG_GREEN        "\033[42m"
#define BG_YELLOW       "\033[43m"
#define BG_BLUE         "\033[44m"
#define BG_MAGENTA      "\033[45m"
#define BG_CYAN         "\033[46m"
#define BG_WHITE        "\033[47m"

#define BOX_TL          "┌" 
#define BOX_TR          "┐" 
#define BOX_BL          "└"
#define BOX_BR          "┘" 
#define BOX_H           "─"
#define BOX_V           "│" 
#define BOX_VR          "├"
#define BOX_VL          "┤"
#define BOX_HU          "┴" 
#define BOX_HD          "┬"
#define BOX_CROSS       "┼"

#define DBOX_TL         "╔" 
#define DBOX_TR         "╗" 
#define DBOX_BL         "╚" 
#define DBOX_BR         "╝" 
#define DBOX_H          "═" 
#define DBOX_V          "║" 
#define DBOX_VR         "╠"
#define DBOX_VL         "╣"

void clearScreen();

// Border & Layout Functions
void printHorizontalLine(int width, const char* left, const char* middle, const char* right);
void printDoubleLine(int width);
void printSingleLine(int width);
void printBoxedText(const char* text, int width);
void printSectionHeader(const char* emoji, const char* title);
void printSectionDivider();

// Breadcrumb Navigation
void printBreadcrumb(const char* path);

// Status Bar
void printStatusBar(const char* username, int karma, const char* location);

// Visual Feedback Messages
void printSuccess(const char* message);
void printError(const char* message);
void printWarning(const char* message);
void printInfo(const char* message);

// Progress Indicators
void showProgress(const char* message, int percent);
void spinnerAnimation(const char* message, int steps);

// Menu System
void printMenu();
void printCommandHelp(const char* command, const char* description, int isActive);

// ASCII Art
void printBanner();

// Loading Bars
void loadingBar(int length, int duration);
void loadingBarSmooth(int length, int delayMicroseconds);

// Input Prompt
void printInputPrompt(const char* context);

#endif