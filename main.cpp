/*
    Testing ncurses
==================================================
    - Amaru M (@oplynx)
    - 6/17/2026
==================================================
*/

#include <ncurses.h>
#include <cctype>
#include <chrono>
#include <thread>
#include <vector>
#include <numeric>
#include <cmath>

#include "utils/constants.h"

/* ======= Object Defintions  ======== */

struct Map 
{
    int height;
    int width;
    std::vector<std::vector<char>> grid; // Logical Buffer Model (LBM)
};

struct Player 
{
    char symbol = '@';
    int x;
    int y;
};

/* Prototype functions */
WINDOW *create_win(int height, int width, int y, int x);
void destroy_win(WINDOW* local_win);
void init_ncurses();
void render_map(WINDOW* win, const Map& map);

int main(int argc, char* argv[]) 
{
    init_ncurses(); // Initialize ncurses

    Map map;
    map.height = default_map_h;
    map.width = default_map_w;
    map.grid = std::vector<std::vector<char>>(map.height, std::vector<char>(map.width, '*'));

    Player player;
    player = {'@', default_start_x, default_start_y};

    map.grid[player.y][player.x] = player.symbol;

    int win_w = (map.width*2) + 2;
    WINDOW *game_win = create_win(map.height + 2, win_w, 2, 5);   
   
    // Init map render so player shows up immediately
    render_map(game_win, map);

    int ch;
    
    while((ch = getch()) != 'q' && ch != 'Q') // Exit if it's 'q' or 'Q'
    {
        /* Clear old player positions in the grid */
        map.grid[player.y][player.x] = '.';

        // Input Handeling
        switch(ch) 
        {
            case KEY_UP:    case 'w': case 'W': if (player.y > 0) player.y--; break;
            case KEY_DOWN:  case 's': case 'S': if (player.y < map.height - 1) player.y++; break;
            case KEY_LEFT:  case 'a': case 'A': if (player.x > 0) player.x--; break;
            case KEY_RIGHT: case 'd': case 'D': if (player.x < map.width - 1) player.x++; break;
        }

        /* Insert player into new position in the grid */
        map.grid[player.y][player.x] = player.symbol;
        /* render grid in ncurses window */
        render_map(game_win, map);
    }

    destroy_win(game_win);
    endwin(); // Deallocate memory and exit ncurses mode
    return 0;
}

/* Initialize NCurses Mode */
void init_ncurses() 
{
    initscr();
    // ncurse Setup Functions
    cbreak(); /* Disables line buffering so userinput is instant (not Enter Key required) */
    noecho(); /* Prevents characters typed by user from being printed automatically on screen */
    keypad(stdscr, TRUE); // Enables special keys
    curs_set(0); // Hides blinking terminal cursor
}

/* Copies the LBM vector to the Display buffer window */
void render_map(WINDOW* win, const Map& map) 
{
    // Clear the inside of the window, then redraw the border
    wclear(win);
    box(win, 0,0);

    for (int y = 0; y < map.height; ++y) {
        for (int x = 0; x < map.width; ++x) {
            char curr_ch = map.grid[y][x];

            int screen_x1 = (x*2) + 1;
            int screen_x2 = (x*2) + 2;
            int screen_y = y + 1;

            if (curr_ch == '@') {
                // If it's the player, print a padded block or pair to look symmetrical
                mvwaddch(win, screen_y, screen_x1, '[');
                mvwaddch(win, screen_y, screen_x2, ']');
            } else {
                // For standard tiles, print them twice side-by-side to make a square block
                mvwaddch(win, screen_y, screen_x1, curr_ch);
                mvwaddch(win, screen_y, screen_x2, curr_ch);
            }
        }
    }

    wrefresh(win); // minimize draw calls
}

/* Function for Creating Windows of parameters heihgt, width, positions y and x */
WINDOW *create_win(int height, int width, int y, int x) 
{
    WINDOW *local_win; // Display Buffer (View)

    local_win = newwin(height, width, y, x); 
    box(local_win, 0, 0);   /* 0,0 gives default characters 
                            * for the vertical and horizonal lines */                  
    wrefresh(local_win); // Show the box

    return local_win;
}

void destroy_win(WINDOW *local_win) 
{
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    /* The parameters mean
     * 1. win: the window on which it operates
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
    */

    wrefresh(local_win);
    wclear(local_win);
    delwin(local_win); // clean window memory allocation
}