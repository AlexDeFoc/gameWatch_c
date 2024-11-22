#include "defines.h"
#include "platform_agnostic.h"
#include "platforms.h"

/*
 * This application is platform agnostic.
 * Platform abstraction arrises when we create a new thread and sleep on it while listening for user input.
 * This application currently supports recording time while still accepting user input only on Windows.
 * On other platforms then Windows sleeping blocks the main thread but still records time. Although requiring forcefully closing the app to stop.
*/

/*
 * Description of app menu options:
   - List (List the current games saved in watch file and the time recorded in each entry).
   - Add (Add a new game title).
   - Remove (Remove a game title).
   - Start (Start recording time to for the selected game title. Time is recorded in intervals of 5 minutes).
*/

int main(void)
{
    // Start main loop.
    while(1) {
        // Print menu options.
        print_menu_options();
        // Get user input.
        u16 user_number = get_user_number(TOTAL_MENU_OPTIONS);
        // Decide which path the user takes.
        if (user_number == MO_LIST) {
            list_game_titles();
        } else if (user_number == MO_ADD) {
            add_game_title();
        } else if (user_number == MO_REMOVE) {
            remove_game_title();
        } else if (user_number == MO_START) {
            printf("You have chosen START!\n");
        }
    }

    return 0;
}
