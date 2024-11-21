#include "keyboard.h"
#include "screen.h"

// Variables
char nickname[20] = {"Mr. Fresh"}, ip[20];
bool exit_program = false, confirmations[2] = {false, false};
unsigned char current_screen = 0;

int main() {
  // Init screen
  setup();

  // Loop
  while (!exit_program) {
    clear_screen();

    // Draw screen
    switch(current_screen) {
      case 0:
        draw_menu();
        break;
      case 1:
        draw_connection();
        break;
      case 2:
        draw_settings();
        break;
    }

    // Get user input
    get_key();
  }
  exit_game();
  return 0;
}
