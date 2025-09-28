
// Portable Snake Game in C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

#define HEIGHT 20
#define WIDTH 40

int snakeTailX[100], snakeTailY[100];
int snakeTailLen;
int gameover, key, score;
int x, y, fruitx, fruity;

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// ------------ Input Handling ----------------
#ifndef _WIN32
// Enable terminal mode
void enableRawMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Restore terminal mode
void disableRawMode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Check if key pressed (non-blocking)
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
#endif

// ------------ Game Setup ----------------
void setup() {
    gameover = 0;
    x = WIDTH / 2;
    y = HEIGHT / 2;

    srand(time(NULL));
    fruitx = rand() % WIDTH;
    fruity = rand() % HEIGHT;

    score = 0;
    snakeTailLen = 0;
}

// ------------ Draw Game Field ----------------
void draw() {
    system(CLEAR);

    // Top wall
    for (int i = 0; i < WIDTH + 2; i++) printf("-");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            if (j == 0 || j == WIDTH)
                printf("#");
            else if (i == y && j == x)
                printf("O");
            else if (i == fruity && j == fruitx)
                printf("*");
            else {
                int printTail = 0;
                for (int k = 0; k < snakeTailLen; k++) {
                    if (snakeTailX[k] == j && snakeTailY[k] == i) {
                        printf("o");
                        printTail = 1;
                    }
                }
                if (!printTail) printf(" ");
            }
        }
        printf("\n");
    }

    // Bottom wall
    for (int i = 0; i < WIDTH + 2; i++) printf("-");
    printf("\n");

    printf("Score = %d\n", score);
    printf("Use W/A/S/D to move, X to quit.\n");
}

// ------------ Input ----------------
void input() {
#ifdef _WIN32
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': case 'A': if (key != 2) key = 1; break;
        case 'd': case 'D': if (key != 1) key = 2; break;
        case 'w': case 'W': if (key != 4) key = 3; break;
        case 's': case 'S': if (key != 3) key = 4; break;
        case 'x': case 'X': gameover = 1; break;
        }
    }
#else
    if (kbhit()) {
        char ch = getchar();
        switch (ch) {
        case 'a': case 'A': if (key != 2) key = 1; break;
        case 'd': case 'D': if (key != 1) key = 2; break;
        case 'w': case 'W': if (key != 4) key = 3; break;
        case 's': case 'S': if (key != 3) key = 4; break;
        case 'x': case 'X': gameover = 1; break;
        }
    }
#endif
}

// ------------ Logic ----------------
void logic() {
    int prevX = snakeTailX[0];
    int prevY = snakeTailY[0];
    int prev2X, prev2Y;

    snakeTailX[0] = x;
    snakeTailY[0] = y;

    for (int i = 1; i < snakeTailLen; i++) {
        prev2X = snakeTailX[i];
        prev2Y = snakeTailY[i];
        snakeTailX[i] = prevX;
        snakeTailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (key) {
    case 1: x--; break;
    case 2: x++; break;
    case 3: y--; break;
    case 4: y++; break;
    default: break;
    }

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        gameover = 1;

    for (int i = 0; i < snakeTailLen; i++) {
        if (snakeTailX[i] == x && snakeTailY[i] == y)
            gameover = 1;
    }

    if (x == fruitx && y == fruity) {
        fruitx = rand() % WIDTH;
        fruity = rand() % HEIGHT;
        score += 10;
        snakeTailLen++;
    }
}

// ------------ Main ----------------
int main() {
#ifndef _WIN32
    enableRawMode();
#endif

    setup();

    while (!gameover) {
        draw();
        input();
        logic();

#ifdef _WIN32
        Sleep(100);
#else
        usleep(100000);
#endif
    }

#ifndef _WIN32
    disableRawMode();
#endif

    printf("\nGame Over! Final Score: %d\n", score);
    return 0;
}
