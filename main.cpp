#include <ncurses.h>

/* Prototype functions */
void init_ncurses();

/* Run Main */
int main(int argc, char* argv[]) {
    init_ncurses();
    printw("Hello World!");
    refresh();
    getch();
    endwin();
    return 0;
}

void init_ncurses() {
    initscr();
}