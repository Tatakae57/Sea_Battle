#include "keyboard.h"
#include <ncurses.h>
#include <cstring>
#include "client.h"

// External variables
extern unsigned char current_option, current_screen;
extern bool exit_program;
extern char nickname[20], ip[20], confirm[2];
extern bool confirmations[2];

// Functions
static void menu_key(const unsigned short &key) {
  switch (key) {
    //   Arrows
    // Down or Right
    case KEY_DOWN:
    case KEY_RIGHT:
      if (current_option != 2) current_option++;
      break;

    // Up or Left
    case KEY_UP:
    case KEY_LEFT:
      if (current_option != 0) current_option--;
      break;

    //   Others
    // Escape
    case 27:
      exit_program = true;
      break;

    // Enter
    case 10:
      // Clean confirmations
      if (confirmations[0]) {
        confirmations[0] = false;
        confirmations[1] = false;
        confirm[0] = 0;
        confirm[1] = 0;
      } 
      // Connect option
      if (!current_option) {curs_set(TRUE); current_screen = 1;}
      // Settings option
      else if (current_option == 1) {curs_set(TRUE); current_screen = 2;}
      // Exit option
      else if (current_option == 2) exit_program = true;
  }
}

static void text_input(const unsigned short &key, char input[20]) {
  switch(key) {
    // Enter
    case 10:
      if (current_screen == 2) { // Enter nickname
        curs_set(FALSE);
        current_screen = 0;
      }
      else { // Connect to server
        if (confirmations[0] == true) {
          confirmations[1] = false;
          confirm[0] = 0;
          confirm[1] = 0;
        }
        else confirmations[0] = true;
        create_socket();
      }
      break;
    // Escape
    case 27:
      curs_set(FALSE);
      current_screen = 0;
      break;
    // Delete
    case KEY_BACKSPACE:
      input[strlen(input) - 1] = '\0';
      break;
    // Enter chars
    default:
      if ((strncmp(keyname(key), "KEY", 3) != 0) && strlen(input) + strlen(keyname(key)) <= 20) // Verify the input is not key  and not out of limits
        sprintf(input, "%s%s", input, keyname(key)); // Adding chars to input
      break;
  }
}

static void setup_key(const unsigned short &key) {
  switch (key) {
    // Enter (send ready message)
    case 10:
      break;
    // Delete (grab boat)
    case KEY_BACKSPACE:
      break;
    // Space (change boat position)
    case 32:
      break;

    // Move boat
    case KEY_UP:
      break;
    case KEY_DOWN:
      break;
    case KEY_RIGHT:
      break;
    case KEY_LEFT:
      break;
  }
}

static void game_key(const unsigned short &key) {
  switch(key) {
    // Move cursor
    case KEY_UP:
      break;
    case KEY_DOWN:
      break;
    case KEY_RIGHT:
      break;
    case KEY_LEFT:
      break;

    // Shoot in selected position (enter)
    case 10:
      break;
  }
}

void get_key() {
  const unsigned short key = getch(); // Wait for keyboard input
  flushinp(); // Clear input

  // Menu
  if (current_screen == 0) menu_key(key);
  // Connect to game
  else if (current_screen == 1) text_input(key, ip);
  // Settings
  else if (current_screen == 2) text_input(key, nickname);
  // Set your boats
  else if (current_screen == 3) setup_key(key);
}
