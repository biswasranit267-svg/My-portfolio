#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 30
#define HEIGHT 15
#define MAX_SNAKE 100

int snakeX[MAX_SNAKE];
int snakeY[MAX_SNAKE];

int snakeLength = 3;

int foodX;
int foodY;

int gameOver = 0;

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum Direction direction = RIGHT;


// Generate food at a random position
void createFood() {
    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;
}


// Draw the game
void drawGame()
//Game speed
usleep(200000); {

    clear();

    // Top border
    for (int i = 0; i < WIDTH + 2; i++)
        printw("#");

    printw("\n");

    for (int y = 0; y < HEIGHT; y++) {

        printw("#");

        for (int x = 0; x < WIDTH; x++) {

            if (x == snakeX[0] && y == snakeY[0]) {
                printw("O");       // Snake head
            }
            else if (x == foodX && y == foodY) {
                printw("*");       // Food
            }
            else {

                int printed = 0;

                for (int i = 1; i < snakeLength; i++) {
                    if (x == snakeX[i] && y == snakeY[i]) {
                        printw("o");   // Snake body
                        printed = 1;
                        break;
                    }
                }

                if (!printed)
                    printw(" ");
            }
        }

        printw("#\n");
    }

    // Bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        printw("#");

    printw("\nScore: %d", snakeLength - 3);
    refresh();
}


// Check collision
void checkCollision() {

    // Wall collision
    if (snakeX[0] < 0 ||
        snakeX[0] >= WIDTH ||
        snakeY[0] < 0 ||
        snakeY[0] >= HEIGHT) {

        gameOver = 1;
    }

    // Body collision
    for (int i = 1; i < snakeLength; i++) {

        if (snakeX[0] == snakeX[i] &&
            snakeY[0] == snakeY[i]) {

            gameOver = 1;
        }
    }
}


// Move snake
void moveSnake() {

    // Move body
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    // Move head
    if (direction == UP)
        snakeY[0]--;

    else if (direction == DOWN)
        snakeY[0]++;

    else if (direction == LEFT)
        snakeX[0]--;

    else if (direction == RIGHT)
        snakeX[0]++;
}


// Check if snake ate food
void checkFood() {

    if (snakeX[0] == foodX &&
        snakeY[0] == foodY) {

        if (snakeLength < MAX_SNAKE)
            snakeLength++;

        createFood();
    }
}


int main() {

    srand(time(NULL));

    // Starting position
    snakeX[0] = 10;
    snakeY[0] = 7;

    snakeX[1] = 9;
    snakeY[1] = 7;

    snakeX[2] = 8;
    snakeY[2] = 7;

    createFood();

    // Start ncurses
    initscr();

    // Don't wait for keyboard input
    nodelay(stdscr, TRUE);

    // Allow arrow keys
    keypad(stdscr, TRUE);

    // Hide cursor
    curs_set(0);

    while (!gameOver) {

        int key = getch();

        // Change direction
        if (key == KEY_UP && direction != DOWN)
            direction = UP;

        else if (key == KEY_DOWN && direction != UP)
            direction = DOWN;

        else if (key == KEY_LEFT && direction != RIGHT)
            direction = LEFT;

        else if (key == KEY_RIGHT && direction != LEFT)
            direction = RIGHT;

        // Move
        moveSnake();

        // Check food
        checkFood();

        // Check collision
        checkCollision();

        // Draw
        drawGame();

        // Game speed
        usleep(120000);
    }

    // End ncurses
    nodelay(stdscr, FALSE);

    clear();

    printw("GAME OVER!\n");
    printw("Final Score: %d\n", snakeLength - 3);
    printw("Press any key to exit...");

    refresh();

    getch();

    endwin();

    return 0;
