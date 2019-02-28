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
#define BG_PURPLE_FG_WHITE "\033[0m\033[97;45m"
#define BG_PURPLE_BRIGHT_FG_WHITE "\033[0m\033[97;105m"

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
enum GAME_STATE {GAME_QUIT, GAME_SPLASH_MENU, GAME_HELP, GAME_PLAYER_SELECTION, GAME_MAIN} GAME_STATE;

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

// Because it's no fun using just ascii
size_t utf8_strlen(const char *s) {
    size_t count = 0;
    while (*s) {
        count += (*s++ & 0xC0) != 0x80;
    }
    return count;
}

int getConsoleWidth() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
}

int getConsoleHeight() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_row;
}

void center(char *s, int w, char *format, int isWithFormatting, int actualLength) {
        int stringLength = 0;
        if (isWithFormatting == 1) {stringLength = actualLength;} else {stringLength = utf8_strlen(s);}
        int padding = (w-stringLength)/2;
        // printf("%d, %d, %d", w, strlen(s), padding);
        printf("%s", format);
        for (int i = 0; i < padding; i++) { printf(" "); }
        printf("%s", s);
        for (int i = 0; i < padding; i++) { printf(" "); }
        printf("%s\n", format);
}

int printMainMenu(int width, int selectedMenuItem) {
        int menuSize = sizeof(splashGameMenu) / sizeof(splashGameMenu[0]);
        for (int i = 0; i < menuSize; i++) {
                char menuString[BUFFER_LEN];
                if (selectedMenuItem == i) {
                        sprintf(menuString, "> %s <", splashGameMenu[i]);
                        center(menuString, width, BG_BLUE_FG_YELLOW, 0, 0);
                } else {
                        sprintf(menuString, "%s", splashGameMenu[i]);
                        center(menuString, width, BG_BLUE_FG_WHITE, 0, 0);
                }
        }
        return menuSize;
}

int printPlayerNumberButtons(int width, int selectedNumOfPlayers) {
        char top[BUFFER_LEN], middle[BUFFER_LEN], bottom[BUFFER_LEN]; 
        char one[BUFFER_LEN] = BG_PURPLE_FG_WHITE;
        char two[BUFFER_LEN] = BG_PURPLE_FG_WHITE;
        char three[BUFFER_LEN] = BG_PURPLE_FG_WHITE;
        char four[BUFFER_LEN] = BG_PURPLE_FG_WHITE; 
        char reset[BUFFER_LEN] = BG_PURPLE_FG_WHITE;
        switch(selectedNumOfPlayers) {
                case 1:
                        strcpy(one, BG_PURPLE_BRIGHT_FG_WHITE);
                        break;
                case 2:
                       strcpy(two, BG_PURPLE_BRIGHT_FG_WHITE);
                       break;
                case 3:
                       strcpy(three, BG_PURPLE_BRIGHT_FG_WHITE);
                       break;
                case 4:
                        strcpy(four, BG_PURPLE_BRIGHT_FG_WHITE);
                        break;
        }
        sprintf(top,    "%s╔═══════╗%s     %s╔═══════╗%s     %s╔═══════╗%s     %s╔═══════╗%s", one, reset, two, reset, three, reset, four, reset);
        sprintf(middle, "%s║   1   ║%s     %s║   2   ║%s     %s║   3   ║%s     %s║   4   ║%s", one, reset, two, reset, three, reset, four, reset);
        sprintf(bottom, "%s╚═══════╝%s     %s╚═══════╝%s     %s╚═══════╝%s     %s╚═══════╝%s", one, reset, two, reset, three, reset, four, reset);
        center(top, width, BG_PURPLE_FG_WHITE, 1, 51);
        center(middle, width, BG_PURPLE_FG_WHITE, 1, 51);
        center(bottom, width, BG_PURPLE_FG_WHITE, 1, 51);
        return 3;
}

void printJeopardyLogo(int width, char* colour) {
        // WHOA LOOK ITS ASCII ART
        center("        88  88888888888  ,ad8888ba,    88888888ba      db         88888888ba   88888888ba,  8b        d8  ", width, colour, 0, 0);
        center("        88  88          d8\"\'    `\"8b   88      \"8b    d88b        88      \"8b  88      `\"8b  Y8,    ,8P   ", width, colour, 0, 0);
        center("        88  88         d8\'        `8b  88      ,8P   d8\'`8b       88      ,8P  88        `8b  Y8,  ,8P    ", width, colour, 0, 0);
        center("        88  88aaaaa    88          88  88aaaaaa8P\'  d8\'  `8b      88aaaaaa8P\'  88         88   \"8aa8\"     ", width, colour, 0, 0);
        center("        88  88\"\"\"\"\"    88          88  88\"\"\"\"\"\"\'   d8YaaaaY8b     88\"\"\"\"88\'    88         88    `88\'      ", width, colour, 0, 0);
        center("        88  88         Y8,        ,8P  88         d8\"\"\"\"\"\"\"\"8b    88    `8b    88         8P     88       ", width, colour, 0, 0);
        center("88,   ,d88  88          Y8a.    .a8P   88        d8\'        `8b   88     `8b   88      .a8P      88       ", width, colour, 0, 0);
        center(" \"Y8888P\"   88888888888  `\"Y8888Y\"\'    88       d8\'          `8b  88      `8b  88888888Y\"\'       88       ", width, colour, 0, 0);
}

void printHelpScreen(int width, int height) {
        printf("%s\n", BG_BLUE_FG_WHITE);
        VERTICAL_PADDING((height/2) - 8);
        printJeopardyLogo(width, BG_BLUE_FG_WHITE);
        VERTICAL_PADDING(4);
        center("Help Menu!", width, BG_BLUE_FG_WHITE, 0, 0);
        center("You really don't need help.", width, BG_BLUE_FG_WHITE, 0, 0);
        center("Press [q] to exit to main menu", width, BG_BLUE_FG_WHITE, 0, 0);
        VERTICAL_PADDING((height/2) - 7);
}

void printSplashScreen(int width, int height, int selectedMenuItem) {
        printf("%s\n", BG_BLUE_FG_WHITE);
        VERTICAL_PADDING((height/2) - 8);
        printJeopardyLogo(width, BG_BLUE_FG_WHITE);
        VERTICAL_PADDING(6);
        int menuHeight = printMainMenu(width, selectedMenuItem);
        VERTICAL_PADDING((height/2) - (menuHeight+6));
}

void printPlayerCountScreen(int width, int height, int selectedNumOfPlayers) {
        printf("%s\n", BG_PURPLE_FG_WHITE);
        VERTICAL_PADDING((height/4) - 8);
        printJeopardyLogo(width, BG_PURPLE_FG_WHITE);
        VERTICAL_PADDING(height/4);
        center("How many plyers?", width, BG_PURPLE_FG_WHITE, 0, 0);
        VERTICAL_PADDING(3);
        int player_button_lines = printPlayerNumberButtons(width, selectedNumOfPlayers);
        if (selectedNumOfPlayers != 0) {
                VERTICAL_PADDING(3);
                center("Press [SPACE] to confirm", width, BG_PURPLE_FG_WHITE, 0, 0);
                VERTICAL_PADDING((height/2) - (player_button_lines + 8));
        } else {
                VERTICAL_PADDING((height/2) - (player_button_lines + 4));
        }
        
}

void printPlayerNameEntryScreen(int width, int height, int playerNum) {
        char outstr[BUFFER_LEN];
        printf("%s\n", BG_PURPLE_FG_WHITE);
        VERTICAL_PADDING((height/4) - 8);
        printJeopardyLogo(width, BG_PURPLE_FG_WHITE);
        VERTICAL_PADDING(height/4);
        sprintf(outstr, "WHAT'S YOUR NAME, PLAYER %d?", playerNum);
        center(outstr, width, BG_PURPLE_FG_WHITE, 0, 0);
        VERTICAL_PADDING((height/2) - 1);
}

int main(int argc, char *argv[]) {
        printf("\033[?47h\n");

        int isRunning = 1; // is looping
        int gameState = GAME_SPLASH_MENU; // Based on GAME_STATE enum
        int mainMenuSelectedItem = 0; // splash menu selected item

        int playerSelectState = 0;
        int selectedNumOfPlayers = 0;
        int numOfPlayers = 0;

        // Main loop
        while(isRunning) {
                switch(gameState) {
                        case GAME_QUIT:
                                isRunning = 0;
                                break;
                        case GAME_HELP:
                                printf("\033[1;1H");
                                printHelpScreen(getConsoleWidth(), getConsoleHeight());
                                printf("\033[1;1H");
                                switch (getch()) {
                                        case KEY_QUIT:
                                                gameState = GAME_SPLASH_MENU;
                                                break;
                                }
                                printf(".\033[0m\033[2J");
                                break;
                        case GAME_SPLASH_MENU:
                                printf("\033[1;1H");
                                printSplashScreen(getConsoleWidth(), getConsoleHeight(), mainMenuSelectedItem);
                                printf("\033[1;1H");
                                char ch = getch();
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
                                                        case 0: gameState = GAME_PLAYER_SELECTION; break;
                                                        case 1: gameState = GAME_HELP; break;
                                                        case 2: gameState = GAME_QUIT; break;
                                                }
                                                break;
                                        default:
                                                printf("\n\n[%d]\n\n", ch);
                                                getch();
                                                break;
                                }
                                printf(".\033[0m\033[2J");
                                break;
                        case GAME_PLAYER_SELECTION:
                                switch(playerSelectState) {
                                        case 0:
                                                printf("\033[1;1H");
                                                printPlayerCountScreen(getConsoleWidth(), getConsoleHeight(), selectedNumOfPlayers);
                                                printf("\033[1;1H");
                                                switch (getch()) {
                                                        case 49:
                                                                selectedNumOfPlayers = 1;
                                                                break;
                                                        case 50:
                                                                selectedNumOfPlayers = 2;
                                                                break;
                                                        case 51:
                                                                selectedNumOfPlayers = 3;
                                                                break;
                                                        case 52:
                                                                selectedNumOfPlayers = 4;
                                                                break;
                                                        case KEY_SELECT:
                                                                if (selectedNumOfPlayers != 0) {
                                                                        playerSelectState++;
                                                                }
                                                                break;
                                                        case KEY_QUIT:
                                                                gameState = GAME_SPLASH_MENU;
                                                                break;
                                                }
                                                break;
                                        case 1:
                                                printf("\033[1;1H");
                                                printPlayerNameEntryScreen(getConsoleWidth(), getConsoleHeight());
                                                printf("\033[1;1H");
                                                switch (getch()) {
                                                        case KEY_QUIT:
                                                                gameState = GAME_SPLASH_MENU;
                                                                break;
                                                }
                                                break;
                                }
                                printf(".\033[0m\033[2J");
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