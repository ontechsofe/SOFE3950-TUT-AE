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
#define BG_BLACK_FG_GREEN "\033[0m\033[92;40m"
#define BG_BLUE_FG_YELLOW "\033[0m\033[93;44m"
#define BG_BLUE_FG_RED "\033[0m\033[91;44m"
#define BG_PURPLE_FG_WHITE "\033[0m\033[97;45m"
#define BG_PURPLE_BRIGHT_FG_WHITE "\033[0m\033[97;105m"
#define TEXTBOX "\033[0m\033[96;100m"

// Vertical Padding Macro
#define VERTICAL_PADDING(x) for(int i = 0; i < x; i++) {for (int j = 0; j < width; j++) {printf(" ");}printf("\n");}
#define HORIZONTAL_PADDING(x) for (int i = 0; i < x; i++) {printf(" ");}

//Key-bind for interface movement
#define KEY_LEFT 104
#define KEY_RIGHT 108
#define KEY_UP 107
#define KEY_DOWN 106
#define KEY_QUIT 113
#define KEY_SELECT 32
#define KEY_ENTER 10
//buffer 
#define BUFFER_LEN 256

// game consts
#define PLAYSOUND 0

// Global vars
enum GAME_STATE {GAME_QUIT, GAME_SPLASH_MENU, GAME_HELP, GAME_PLAYER_SELECTION, GAME_BOARD,GAME_QUESTION,GAME_PLAYERS} GAME_STATE;

struct Player {
        char name[BUFFER_LEN];
        int score;
};

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

void sayThis(char *s) {
        if (PLAYSOUND == 1) {
                char buf[BUFFER_LEN];
                sprintf(buf, "./tts/gnuspeech_sa -c ./tts/data/en -p /tmp/jeopardy.txt -o /tmp/jeopardy.wav \"%s\" && aplay -q /tmp/jeopardy.wav", s);
                system(buf);
        }
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

void centerNoNewline(char *s, int w, char *format, int isWithFormatting, int actualLength) {
        int stringLength = 0;
        if (isWithFormatting == 1) {stringLength = actualLength;} else {stringLength = utf8_strlen(s);}
        int padding = (w-stringLength)/2;
        // printf("%d, %d, %d", w, strlen(s), padding);
        printf("%s", format);
        HORIZONTAL_PADDING(padding);
        printf("%s", s);
        HORIZONTAL_PADDING(padding);
        printf("%s", format);
}

void center(char *s, int w, char *format, int isWithFormatting, int actualLength) {
        centerNoNewline(s, w, format, isWithFormatting, actualLength);
        printf("\n");
}

void printTextbox(char *s, int w, int length) {
        int padding = (w - length) / 2;
        int stlen = utf8_strlen(s);
        HORIZONTAL_PADDING(padding);
        printf("%s%s", TEXTBOX, s);
        HORIZONTAL_PADDING(length - stlen);
        printf("%s", BG_PURPLE_FG_WHITE);
        HORIZONTAL_PADDING(padding);
        printf("\n");
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
        VERTICAL_PADDING(3);
        center("use [J,K] to select, and [ENTER] to confirm", width, BG_BLUE_FG_WHITE, 0, 0);
        VERTICAL_PADDING((height/2) - (menuHeight + 10));
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
                center("Press [ENTER] to confirm", width, BG_PURPLE_FG_WHITE, 0, 0);
                VERTICAL_PADDING((height/2) - (player_button_lines + 8));
        } else {
                VERTICAL_PADDING((height/2) - (player_button_lines + 4));
        }
        
}

void printPlayerNameEntryScreen(int width, int height, int playerNum, char* name) {
        char outstr[BUFFER_LEN];
        printf("%s\n", BG_PURPLE_FG_WHITE);
        VERTICAL_PADDING((height/4) - 8);
        printJeopardyLogo(width, BG_PURPLE_FG_WHITE);
        VERTICAL_PADDING(height/4);
        sprintf(outstr, "WHAT'S YOUR NAME, PLAYER %d?", playerNum + 1);
        center(outstr, width, BG_PURPLE_FG_WHITE, 0, 0);
        VERTICAL_PADDING(4);
        printTextbox(name, width, 30);
        VERTICAL_PADDING(3);
        center("Press [ENTER] to confirm name", width, BG_PURPLE_FG_WHITE, 0, 0);
        VERTICAL_PADDING((height/2) - 10);
}

void printPlayerConfirmScreen(int width, int height, int numPlayers, struct Player players[]) {
        printf("%s\n", BG_PURPLE_FG_WHITE);
        VERTICAL_PADDING((height/4) - 8);
        printJeopardyLogo(width, BG_PURPLE_FG_WHITE);
        VERTICAL_PADDING(height/4);
        center("Does everything look good??", width, BG_PURPLE_FG_WHITE, 0, 0);
        VERTICAL_PADDING(3);
        int tablePadding = 2;
        center("╔══════════╤════════════════════════════════╗", width, BG_PURPLE_FG_WHITE, 0, 0);
        for (int i = 0; i < numPlayers; i++) {
                char curNameBuf[BUFFER_LEN];
                sprintf(curNameBuf, "║ Player %d │ %-30s ║", (i+1), players[i].name);
                center(curNameBuf, width, BG_PURPLE_FG_WHITE, 0, 0);
                tablePadding++;
                if (i != numPlayers - 1) {
                        tablePadding++;
                        center("╟──────────┼────────────────────────────────╢", width, BG_PURPLE_FG_WHITE, 0, 0);
                }
        }
        center("╚══════════╧════════════════════════════════╝", width, BG_PURPLE_FG_WHITE, 0, 0);
        VERTICAL_PADDING(3);
        center("Press [ENTER] to confirm or [Q] to go back", width, BG_PURPLE_FG_WHITE, 0, 0);
        VERTICAL_PADDING((height/2) - (8 + tablePadding));
}

void printGameBoard(int width, int height, int selectedX, int selectedY, char theBoard[7][7][BUFFER_LEN]) {
        printf("%s", BG_BLUE_FG_WHITE);
        int w = width / 6;
        VERTICAL_PADDING(5);
        for (int j = 0; j < 6; j++) {
                printf("%-*s", w, theBoard[j][0]);
        }
        printf("\n");
        VERTICAL_PADDING(5);
        for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 6; j++) {
                        if (i == selectedY && j == selectedX) {
                                printf("%s", BG_BLUE_FG_RED);
                        }
                        printf("%-*d", w, (200 * (i+1)));
                        printf("%s", BG_BLUE_FG_WHITE);
                }
                printf("\n");
                VERTICAL_PADDING(5);
        }
}

void printMainGameScreen(int width, int height, int selectedX, int selectedY, char theBoard[7][7][BUFFER_LEN]) {
        printf("%s", BG_BLUE_FG_WHITE);
        printGameBoard(width, height, selectedX, selectedY, theBoard);
}

void printGameBoardScreen(int width, int height, int selectedX, int selectedY, char theBoard[7][7][BUFFER_LEN]) {
        printf("%s\n", BG_BLUE_FG_WHITE);
        printGameBoard(width, height, selectedX, selectedY, theBoard);
}

int main(int argc, char *argv[]) {
        printf("\033[?47h\n");

        char* gameFile = "./questions/1.q";

        int isRunning = 1; // is looping
        int hasSaidIntro = 0;
        int gameState = GAME_SPLASH_MENU; // Based on GAME_STATE enum
        int mainMenuSelectedItem = 0; // splash menu selected item

        int playerSelectState = 0;
        int selectedNumOfPlayers = 0;

        char playerNames[4][BUFFER_LEN] = {"", "", "", ""};
        struct Player players[4];
        int numOfPlayers = 0;

        int currentPlayerNameEntry = 0;
        char currentPlayerName[BUFFER_LEN];

        char gameBoardQuestions[7][7][BUFFER_LEN];
        char gameBoardAnswers[7][7][BUFFER_LEN];
        int gameBoardStats[6][6] = {0};

        int boardSelectionX = 0;
        int boardSelectionY = 0;

        for (int i = 0; i < 4; i++) {
                players[i].score = 0;
        }


        FILE *f = fopen(gameFile, "r");
        char readingLine[BUFFER_LEN];
        int lineCounter = 0;
        int categoryCounter = 0;
        int questionCounter = 0;
        while(fgets(readingLine, sizeof(readingLine), f)) {
                if (lineCounter % 6 == 0) {
                        char *category;
                        const char delim[4] = "\n";
                        category = strtok(readingLine, delim);
                        strcpy(gameBoardQuestions[categoryCounter][0], category);
                        strcpy(gameBoardAnswers[categoryCounter][0], category);
                        questionCounter = 1;
                } else {
                        char *question;
                        char *answer;
                        const char delim[4] = "|*|";
                        question = strtok(readingLine, delim);
                        answer = strtok(NULL, delim);
                        strcpy(gameBoardQuestions[categoryCounter][questionCounter], question);
                        strcpy(gameBoardAnswers[categoryCounter][questionCounter], answer);
                        gameBoardAnswers[categoryCounter][questionCounter][strlen(gameBoardAnswers[categoryCounter][questionCounter]) - 1] = '\0';
                        questionCounter++;
                        if (questionCounter > 5) {
                                categoryCounter++;
                        }
                }
                lineCounter++;
        }

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
                                if (hasSaidIntro == 0) {
                                        sayThis("This. Is. Jeopardy.");
                                        hasSaidIntro = 1;
                                }
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
                                        case KEY_ENTER:
                                                switch (mainMenuSelectedItem) {
                                                        case 0: gameState = GAME_PLAYER_SELECTION; playerSelectState = 0; break;
                                                        case 1: gameState = GAME_HELP; break;
                                                        case 2: gameState = GAME_QUIT; break;
                                                }
                                                break;
                                        // default:
                                        //         printf("\n\n[%d]\n\n", ch);
                                        //         getch();
                                        //         break;
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
                                                        case KEY_ENTER:
                                                                if (selectedNumOfPlayers != 0) {
                                                                        playerSelectState++;
                                                                        numOfPlayers = selectedNumOfPlayers;
                                                                        selectedNumOfPlayers = 0;
                                                                        currentPlayerNameEntry = 0;
                                                                        currentPlayerName[0] = '\0';
                                                                }
                                                                break;
                                                        case KEY_QUIT:
                                                                gameState = GAME_SPLASH_MENU;
                                                                break;
                                                }
                                                break;
                                        case 1:
                                                printf("\033[1;1H");
                                                printPlayerNameEntryScreen(getConsoleWidth(), getConsoleHeight(), currentPlayerNameEntry, currentPlayerName);
                                                printf("\033[1;1H");
                                                char ch = getch();
                                                switch (ch) {
                                                        case KEY_ENTER:
                                                                if (currentPlayerNameEntry == (numOfPlayers - 1)) {
                                                                        playerSelectState++;
                                                                }
                                                                strcpy(players[currentPlayerNameEntry].name, currentPlayerName);
                                                                currentPlayerName[0] = '\0';
                                                                currentPlayerNameEntry++;
                                                                break;
                                                        default:
                                                                if (((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122) || ch == 32) && strlen(currentPlayerName) <= 30) {
                                                                        char cpbuf[BUFFER_LEN] = "";
                                                                        strcpy(cpbuf, currentPlayerName);
                                                                        sprintf(currentPlayerName, "%s%c", cpbuf, ch);
                                                                } else if (ch == 127) {
                                                                        currentPlayerName[strlen(currentPlayerName) - 1] = '\0';
                                                                }
                                                                break;
                                                }
                                                break;
                                        case 2:
                                                printf("\033[1;1H");
                                                printPlayerConfirmScreen(getConsoleWidth(), getConsoleHeight(), numOfPlayers, players);
                                                printf("\033[1;1H");
                                                switch (getch()) {
                                                        case KEY_QUIT:
                                                                gameState = GAME_SPLASH_MENU;
                                                                break;
                                                        case KEY_ENTER:
                                                                gameState = GAME_BOARD;
                                                                sayThis("Welcome everyone. It's time to play jeopardy. Let's look at the categories.");
                                                                break;
                                                }
                                                break;
                                }
                                printf(".\033[0m\033[2J");
                                break;
                        case GAME_BOARD:
                                printf("\033[1;1H");
                                printGameBoardScreen(getConsoleWidth(), getConsoleHeight(), boardSelectionX, boardSelectionY, gameBoardQuestions);
                                printf("\033[1;1H");
                                switch (getch()) {
                                        case KEY_UP:
                                                boardSelectionY--;
                                                if (boardSelectionY < 0) {
                                                        boardSelectionY = 4;
                                                }
                                                break;
                                        case KEY_DOWN:
                                                boardSelectionY++;
                                                if (boardSelectionY > 4) {
                                                        boardSelectionY = 0;
                                                }
                                                break;
                                        case KEY_LEFT:
                                                boardSelectionX--;
                                                if (boardSelectionX < 0) {
                                                        boardSelectionX = 5;
                                                }
                                                break;
                                        case KEY_RIGHT:
                                                boardSelectionX++;
                                                if (boardSelectionX > 5) {
                                                        boardSelectionX = 0;
                                                }
                                                break;
                                        case KEY_ENTER:
                                                break;
                                }
                                break;
                        case GAME_QUESTION:
                                printf("\033[1;1H");
                                // printGameBoardScreen(getConsoleWidth(), getConsoleHeight(), boardSelectionX, boardSelectionY, gameBoardQuestions);
                                printf("\033[1;1H");
                                switch (getch()) {
                                        case 49:
                                                // selectedNumOfPlayers = 1;
                                                break;
                                        case 50:
                                                // selectedNumOfPlayers = 2;
                                                break;
                                        case 51:
                                                // selectedNumOfPlayers = 3;
                                                break;
                                        case 52:
                                                // selectedNumOfPlayers = 4;
                                                break;
                                }
                                break;
                        case GAME_PLAYERS:
                                break;
                       
                }
        }
        // Reset terminal to saved state
        printf("\033[?47l");
        // Completely overwrite everything and "clear" the screen
        printf("\033[2J");
        return EXIT_SUCCESS;
}

