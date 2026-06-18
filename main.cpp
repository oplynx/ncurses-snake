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

struct Map 
{
    int height;
    int width;
    std::vector<std::vector<char>> grid; // Logical Buffer Model (LBM)
};

struct Player {
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
    map.height = 10;
    map.width = 20;
    map.grid = std::vector<std::vector<char>>(map.height, std::vector<char>(map.width, '.'));

    Player player;
    player = {'@', 2, 2};

    map.grid[player.y][player.x] = player.symbol;

    WINDOW *game_win = create_win(map.height + 2, map.width + 2, 2, 5);   
   
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
    werase(win);
    box(win, 0,0);

    for (int y = 0; y < map.height; ++y) {
        for (int x = 0; x < map.width; ++x) {
            /* y+1 and x+1 prevents overwriting the window borders */
            mvwaddch(win, y+1, x+1, map.grid[y][x]);
        }
    }

    wrefresh(win);
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
}