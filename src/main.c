#include "main_functions.h"
#include <stdio.h>

int main(void)
{

    u16 user_number = 0;

    while (1) {
        print_menu_options();
        user_number = get_user_number(menu_options_count-1);
        
        if (user_number == List) {
            list_option();
        } else if (user_number == Add) {
            add_option();
        } else if (user_number == Remove) {
            remove_option();
        } else if (user_number == Start) {
            while (1) start_option();
        }

        printf("\n");
    }

    return 0;
}
