#include <ncurses.h>
#include "screen.h"

// Constants
#define POSITIONS 10
#define BOATS 10

// Structures
struct Boat {
  bool vertical;
  unsigned char size, posx, posy;
};

// Variables
//  Externals
extern char nickname[20], ip[20];
extern bool confirmations[2];

//  Globals
unsigned short x, y;
unsigned char current_option = 0, color, confirm[2] = {0, 0};
char player_table[POSITIONS][POSITIONS], enemy_table[POSITIONS][POSITIONS];
struct Boat player_boats[10];

//  Locals
static const char options[3][16] = {"Connect to game", "Change nickname", "Quit"};

// Functions
//  Setup
void setup() {
  initscr(); // Start screen
  noecho(); // No write input
  curs_set(FALSE); // Dont show cursor
  keypad(stdscr, TRUE); // Activate keypad

  // Color
  start_color();
  init_pair(1, COLOR_BLUE, COLOR_WHITE); // Selected option
  init_pair(2, COLOR_WHITE, COLOR_BLACK); // Unselected option
  init_pair(3, COLOR_WHITE, COLOR_BLUE); // Title
  init_pair(4, COLOR_BLACK, COLOR_WHITE); // Subtitles
  init_pair(5, COLOR_WHITE, COLOR_GREEN); // Boats
}

void default_boats() {
  // Creating boats (4)
  for (unsigned char i = 0; i != 9; i++){
    // Mini boats
    if (i <= 3)
      player_boats[i].size = 1;
    // Boats
    else if (i <= 6)
      player_boats[i].size = 2;
    // Big boats
    else if (i <= 8)
      player_boats[i].size = 3;
    // Large boat
    else
      player_boats[i].size = 4;
    player_boats[i].vertical = false;
  }

  // Setting positions
  // Mini boats
  for (unsigned char boat = 0; boat <= 3; boat++) {
    player_boats[boat].posy = 2;
  }
  player_boats[0].posx = 2;
  player_boats[1].posx = 4;
  player_boats[2].posx = 7;
  player_boats[3].posx = 9;

  // Boats
  for (unsigned char boat = 4; boat <= 6; boat++)
    player_boats[boat].posy = 4;
  player_boats[4].posx = 2;
  player_boats[5].posx = 5;
  player_boats[6].posx = 8;

  // Big boats
  for (unsigned char boat = 7; boat <= 8; boat++)
    player_boats[boat].posy = 6;
  player_boats[7].posx = 2;
  player_boats[8].posx = 7;

  // Large boat
  player_boats[9].posy = 8;
  player_boats[9].posx = 4;
}

// Screen
void clear_screen(){
  getmaxyx(stdscr, y, x); // Get window size
  erase(); // Clear screen
  // Draw box if can
  if (x >= 30 && y >= 32) box(stdscr, 0, 0);
}

void draw_menu() {
  unsigned char spaces = 0;
  
  // Show menu
  attron(COLOR_PAIR(3));
  mvprintw(3, (x / 2) - 5, "Sea Battle");
  attroff(COLOR_PAIR(3));

  // Show options
  for (unsigned char i = 0; i != 3; i++) {
    if (i == current_option) color = 1;
    else color = 2;

    // Counting chars in array
    spaces = 7;
    if (i == 2) spaces = 2;
    
    // Set option color
    attron(COLOR_PAIR(color));
    mvprintw(i + 5, x / 2 - spaces, "%s", options[i]);
    attroff(COLOR_PAIR(color));
  }
}

void draw_settings() {
  // Draw title
  attron(COLOR_PAIR(4));
  mvprintw(3, (x / 2) - 7, "%s", options[1]);
  attroff(COLOR_PAIR(4));
  
  // Draw nickname
  mvprintw(5, x / 2 - 10, "Nick: ");
  attron(COLOR_PAIR(3));
  mvprintw(5, x / 2 - 4, "%s", nickname);
  attroff(COLOR_PAIR(3));
}

void draw_connection() {
  // Draw title
  attron(COLOR_PAIR(4));
  mvprintw(3, (x / 2) - 7, "%s", options[1]);
  attroff(COLOR_PAIR(4));
  
  // Draw tips
  mvprintw(y - 5, 2, "Space: Change boat position.");
  mvprintw(y - 4, 2, "Enter in Set your boats: Ready.");
  mvprintw(y - 3, 2, "Enter in game: Shoot in selected position.");
  
  // Draw connection state
  //  Socket creation
  if (confirmations[0])
    mvprintw(7, x / 2 - 13, "Creating socket...");
  if (confirm[0] == 1)
    mvprintw(7, x / 2 + 6, "created.");
  else if (confirm[0] == 2)
    mvprintw(7, x / 2 + 6, "failed.");

  //  Connection to server 
  if (confirmations[1]) 
    mvprintw(8, x / 2 - 10, "Connecting...");
  if (confirm[1] == 1) {
    mvprintw(8, x / 2 + 4, "connected.");
    mvprintw(9, x / 2 - 15, "Waiting for the other player...");
  }
  else if (confirm[1] == 2)
    mvprintw(8, x / 2 + 4, "failed.");
  
  // Draw ip input
  mvprintw(5, x / 2 - 11, "IP: ");
  attron(COLOR_PAIR(3));
  mvprintw(5, x / 2 - 7, "%s", ip);
  attroff(COLOR_PAIR(3));
}

// Tables
static void draw_player_table() {
  WINDOW *window = newwin(12, 12, 4, x / 2 - 15); // Create new window
  wattron(window, COLOR_PAIR(1)); // Enable window color
  box(window, 0, 0); // Draw box
  
  // Draw table
  for (unsigned char posx = 1; posx != POSITIONS + 1; posx++)
    for (unsigned char posy = 1; posy != POSITIONS + 1; posy++)
      mvwaddch(window, posy, posx, ' ');
  wattroff(window, COLOR_PAIR(1)); // Disable window color
  
  // Draw player boats
  wattron(window, COLOR_PAIR(5));
  for (unsigned char boat = 0; boat != BOATS; boat++) {
    for (unsigned char cube = 0; cube != player_boats[boat].size; cube++) {
      if (player_boats[boat].vertical) {
        mvwaddch(window, player_boats[boat].posy + cube, player_boats[boat].posx, ' ');
      }
      else {
        mvwaddch(window, player_boats[boat].posy, player_boats[boat].posx + cube, ' ');
      }
    }
  }
  wattroff(window, COLOR_PAIR(5));
  
  // Show table
  refresh();
  wrefresh(window);
  delwin(window); // Delete window
}

void draw_boats_setup() {
  default_boats();
  // Draw title
  attron(COLOR_PAIR(4));
  mvprintw(2, x / 2 - 7, "Set your boats");
  attroff(COLOR_PAIR(4));
  
  // Draw table
  draw_player_table();
}

// Exit game
void exit_game() {
  endwin();
}
