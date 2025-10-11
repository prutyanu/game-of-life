#include <ncurses/ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define ALIVE 'O'
#define DEAD '.'
#define MIN_DELAY 10000
#define MAX_DELAY 500000
#define DELAY_STEP 50000

void initialize_field(int field[HEIGHT][WIDTH]);
int read_initial_state(int field[HEIGHT][WIDTH]);
void draw_field(int field[HEIGHT][WIDTH]);
int count_neighbors(int field[HEIGHT][WIDTH], int y, int x);
void update_field(int field[HEIGHT][WIDTH], int new_field[HEIGHT][WIDTH]);
void copy_field(int source[HEIGHT][WIDTH], int dest[HEIGHT][WIDTH]);
int check_valid_input(int rows, int cols);

int main() {
    int field[HEIGHT][WIDTH];
    int new_field[HEIGHT][WIDTH];
    int delay = 100000;
    int running = 1;

    initialize_field(field);
    if (read_initial_state(field) != 0) {
        fprintf(stderr, "Error: Invalid input dimensions\n");
        return 1;
    }

    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    curs_set(0);

    while (running) {
        clear();
        draw_field(field);
        mvprintw(HEIGHT + 1, 0, "Controls: A - faster, Z - slower, Q/ESC - exit");
        mvprintw(HEIGHT + 2, 0, "Speed delay: %d microseconds", delay);
        refresh();

        int ch = getch();
        if (ch == ' ' || ch == 'q' || ch == 'Q' || ch == 27) {
            running = 0;
        } else if (ch == 'a' || ch == 'A') {
            if (delay > MIN_DELAY) {
                delay -= DELAY_STEP;
            }
        } else if (ch == 'z' || ch == 'Z') {
            if (delay < MAX_DELAY) {
                delay += DELAY_STEP;
            }
        }

        update_field(field, new_field);
        copy_field(new_field, field);
        usleep(delay);
    }

    endwin();
    return 0;
}

void initialize_field(int field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            field[i][j] = 0;
        }
    }
}

int read_initial_state(int field[HEIGHT][WIDTH]) {
    int rows = 0;
    int cols = 0;
    char line[WIDTH + 2];

    while (fgets(line, sizeof(line), stdin) != NULL && rows < HEIGHT) {
        cols = 0;
        for (int i = 0; line[i] != '\0' && line[i] != '\n' && cols < WIDTH; i++) {
            if (line[i] == ALIVE || line[i] == '1') {
                field[rows][cols] = 1;
            } else if (line[i] == DEAD || line[i] == '0' || line[i] == ' ') {
                field[rows][cols] = 0;
            } else {
                continue;
            }
            cols++;
        }
        rows++;
    }

    if (!check_valid_input(rows, cols)) {
        return 1;
    }
    return 0;
}

int check_valid_input(int rows, int cols) {
    if (rows > HEIGHT || cols > WIDTH || rows == 0 || cols == 0) {
        return 0;
    }
    return 1;
}

void draw_field(int field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1) {
                mvaddch(i, j, ALIVE);
            } else {
                mvaddch(i, j, DEAD);
            }
        }
    }
}

int count_neighbors(int field[HEIGHT][WIDTH], int y, int x) {
    int count = 0;
    int dy, dx, ny, nx;

    for (dy = -1; dy <= 1; dy++) {
        for (dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) {
                continue;
            }

            ny = (y + dy + HEIGHT) % HEIGHT;
            nx = (x + dx + WIDTH) % WIDTH;

            if (field[ny][nx] == 1) {
                count++;
            }
        }
    }

    return count;
}

void update_field(int field[HEIGHT][WIDTH], int new_field[HEIGHT][WIDTH]) {
    int neighbors;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            neighbors = count_neighbors(field, i, j);

            if (field[i][j] == 1) {
                if (neighbors == 2 || neighbors == 3) {
                    new_field[i][j] = 1;
                } else {
                    new_field[i][j] = 0;
                }
            } else {
                if (neighbors == 3) {
                    new_field[i][j] = 1;
                } else {
                    new_field[i][j] = 0;
                }
            }
        }
    }
}

void copy_field(int source[HEIGHT][WIDTH], int dest[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            dest[i][j] = source[i][j];
        }
    }
}
