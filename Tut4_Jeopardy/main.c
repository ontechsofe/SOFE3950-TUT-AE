/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

// Colour is [FG;BGm
#define BG_BLUE_FG_WHITE "\033[0m\033[97;44m"
#define BG_BLUE_FG_YELLOW "\033[0m\033[93;44m"

// Vertical Padding Macro
#define VERTICAL_PADDING(x) for(int i = 0; i < x; i++) {for (int j = 0; j < width; j++) {printf(" ");}printf("\n");}

//Key-bind for interface movement
#define KEY_LEFT 104
#define KEY_RIGHT 108
#define KEY_UP 107
#define KEY_DOWN 106
#define KEY_QUIT 113
#define KEY_SELECT 32

//buffer 
#define BUFFER_LEN 256

// game consts
#define NUM_PLAYERS 4

// Global vars
enum GAME_STATE {GAME_QUIT, GAME_SPLASH_MENU, GAME_HELP, GAME_MAIN} GAME_STATE;

char *splashGameMenu[] = {"Start Game", "HELP", "Quit"};


// adding getch because linux doesn't have conio
int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}


void center(char *s, int w, char *format) {
        int padding = (w-strlen(s))/2;
        // printf("%d, %d, %d", w, strlen(s), padding);
        printf("%s", format);
        for (int i = 0; i < padding; i++) { printf(" "); }
        printf("%s", s);
        for (int i = 0; i < padding; i++) { printf(" "); }
        if (strlen(s) % 2 != 0) { printf(" "); }
        printf("%s\n", BG_BLUE_FG_WHITE);
}

int printMainMenu(int width, int selectedMenuItem) {
        int menuSize = sizeof(splashGameMenu) / sizeof(splashGameMenu[0]);
        for (int i = 0; i < menuSize; i++) {
                char menuString[BUFFER_LEN];
                if (selectedMenuItem == i) {
                        sprintf(menuString, "> %s <", splashGameMenu[i]);
                        center(menuString, width, BG_BLUE_FG_YELLOW);
                } else {
                        sprintf(menuString, "%s", splashGameMenu[i]);
                        center(menuString, width, BG_BLUE_FG_WHITE);
                }
        }
        return menuSize;
}

void printSplashScreen(int width, int height, int selectedMenuItem) {
        printf("%s\n", BG_BLUE_FG_WHITE);
        VERTICAL_PADDING((height/2) - 8);
        center("        88  88888888888  ,ad8888ba,    88888888ba      db         88888888ba   88888888ba,  8b        d8  ", width, BG_BLUE_FG_WHITE);
        center("        88  88          d8\"\'    `\"8b   88      \"8b    d88b        88      \"8b  88      `\"8b  Y8,    ,8P   ", width, BG_BLUE_FG_WHITE);
        center("        88  88         d8\'        `8b  88      ,8P   d8\'`8b       88      ,8P  88        `8b  Y8,  ,8P    ", width, BG_BLUE_FG_WHITE);
        center("        88  88aaaaa    88          88  88aaaaaa8P\'  d8\'  `8b      88aaaaaa8P\'  88         88   \"8aa8\"     ", width, BG_BLUE_FG_WHITE);
        center("        88  88\"\"\"\"\"    88          88  88\"\"\"\"\"\"\'   d8YaaaaY8b     88\"\"\"\"88\'    88         88    `88\'      ", width, BG_BLUE_FG_WHITE);
        center("        88  88         Y8,        ,8P  88         d8\"\"\"\"\"\"\"\"8b    88    `8b    88         8P     88       ", width, BG_BLUE_FG_WHITE);
        center("88,   ,d88  88          Y8a.    .a8P   88        d8\'        `8b   88     `8b   88      .a8P      88       ", width, BG_BLUE_FG_WHITE);
        center(" \"Y8888P\"   88888888888  `\"Y8888Y\"\'    88       d8\'          `8b  88      `8b  88888888Y\"\'       88       ", width, BG_BLUE_FG_WHITE);
        VERTICAL_PADDING(6);
        int menuHeight = printMainMenu(width, selectedMenuItem);
        VERTICAL_PADDING((height/2) - (menuHeight+6));
}

int main(int argc, char *argv[]) {

        printf("\033[?47h\n");

        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        int isRunning = 1; // is looping
        int gameState = GAME_SPLASH_MENU; // Based on GAME_STATE enum
        int mainMenuSelectedItem = 0; // splash menu selected item

        // Main loop
        while(isRunning) {
                switch(gameState) {
                        case GAME_QUIT:
                                isRunning = 0;
                                break;
                        case GAME_SPLASH_MENU:
                                printf("\033[1;1H");
                                printSplashScreen(w.ws_col, w.ws_row, mainMenuSelectedItem);
                                printf("\033[1;1H");
                                char ch;
                                ch = getch();
                                printf(".\033[0m\033[2J");
                                // key selection using vim shortcuts :)
                                switch (ch) {
                                        case KEY_UP:
                                                if (mainMenuSelectedItem == 0) {
                                                        mainMenuSelectedItem = 2;
                                                } else {
                                                        mainMenuSelectedItem -= 1;
                                                }
                                                break;
                                        case KEY_DOWN:
                                                if (mainMenuSelectedItem == 2) {
                                                        mainMenuSelectedItem = 0;
                                                } else {
                                                        mainMenuSelectedItem += 1;
                                                }
                                                break;
                                        case KEY_SELECT:
                                                switch (mainMenuSelectedItem) {
                                                        case 0: gameState = GAME_MAIN; break;
                                                        case 1: gameState = GAME_HELP; break;
                                                        case 2: gameState = GAME_QUIT; break;
                                                }
                                                break;
                                        // case KEY_QUIT:
                                        //         isRunning = 0;
                                        //         break;
                                }
                                break;
                        case GAME_HELP:
                                printf("HELLO DO U NEED HELP");
                                getch();
                                break;
                        case GAME_MAIN:
                                printf("GAME_MAIN");
                                getch();
                                break;
                }
        }
        // Reset terminal to saved state
        printf("\033[?47l");
        // Completely overwrite everything and "clear" the screen
        printf("\033[2J");
        return EXIT_SUCCESS;
}

// key selection using vim shortcuts :)
// switch (ch) {
//         case KEY_LEFT:
//                 printf("LEFT");
//                 break;
//         case KEY_RIGHT:
//                 printf("RIGHT");
//                 break;
//         case KEY_UP:
//                 printf("UP");
//                 break;
//         case KEY_DOWN:
//                 printf("DOWN");
//                 break;
//         case KEY_QUIT:
//                 isRunning = 0;
//                 break;
// }
