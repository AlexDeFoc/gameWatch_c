#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define GOOD 0
#define BAD 1

#define MENU_OPTIONS_NR 4

typedef enum menu_options {
    List = 1, Start, Add, Remove
} menu_options;

void print_menu_options (void)
{
    // Create menu options.
    const char * options[MENU_OPTIONS_NR] = {
        "List",
        "Start",
        "Add",
        "Remove"
    };

    // Print options.
    for (uint8_t i = 0; i < MENU_OPTIONS_NR; i++)
        printf("%d. %s\n", i+1, options[i]);

    // Exit good.
    return;
}

uint8_t get_user_menu_choice (uint8_t limit, size_t **return_input)
{
    // Create input & default limit.
    uint8_t input_default_limit = 255;
    size_t* input = NULL;
    if (limit == 0) {
        input = malloc(input_default_limit * sizeof(uint8_t));
        if (input == NULL) {
            printf("Out of memory!\n");
            printf("Cannot allocate enough memory for input!\n");
            return BAD;
        }
    }
    else {
        input = malloc(limit * sizeof(uint8_t));
        if (input == NULL) {
            printf("Out of memory!\n");
            printf("Cannot allocate enough memory for input!\n");
            return BAD;
        }
    }

    // Create lenght.
    uint8_t lenght = 0;

    // Create var for next char.
    int8_t c = 0;
    while (1) {
        // Get next char.
        c = getc(stdin);

        // Break if find end char.
        if (c == '\n' || c == EOF) break;

        // Break if reached user predefined limit.
        if (limit != 0 && lenght >= limit) break;

        // Resize if limit is 0.
        if (limit == 0 && lenght >= input_default_limit) {
            size_t* new_input = realloc(input, input_default_limit * 2 * sizeof(uint8_t));
            if (new_input == NULL) {
                printf("Out of memory!\n");
                printf("Cannot allocate enough memory for resizing input!\n");
                free(input);
                return BAD;
            }

            input = new_input;
            input_default_limit *= 2;
        }

        // Add char to input. Increment lenght.
        input[lenght] = c - '0';
        lenght++;
    }

    // Exit good.
    *return_input = input;
    return GOOD;
}

uint8_t get_user_input (uint8_t **return_input)
{
    // Create input & default limit.
    uint8_t limit = 255;
    uint8_t * input = NULL;

    input = malloc((limit + 1) * sizeof(uint8_t));
    if (input == NULL) {
        printf("Out of memory!\n");
        printf("Cannot allocate enough memory for input!\n");
        return BAD;
    }

    // Create lenght.
    uint8_t lenght = 0;

    // Create var for next char.
    int8_t c = 0;
    while (1) {
        // Get next char.
        c = getc(stdin);

        // Break if find end char.
        if (c == '\n' || c == EOF) break;

        // Resize if limit reached.
        if (limit == 0 && lenght >= limit) {
            uint8_t * new_input = realloc(input, ((limit * 2) + 1) * sizeof(uint8_t));
            if (new_input == NULL) {
                printf("Out of memory!\n");
                printf("Cannot allocate enough memory for resizing input!\n");
                free(input);
                return BAD;
            }

            input = new_input;
            limit *= 2;
        }

        // Add char to input. Increment lenght.
        input[lenght] = c;
        lenght++;
    }

    // Add null char at end.
    input[lenght] = '\0';

    // Exit good.
    *return_input = input;
    return GOOD;
}

void list_phase (size_t * return_entries_found)
{
    // Open watch file.
    FILE *watch_p = fopen("./watch.txt", "r");
    if (watch_p == NULL) {
        printf("Watch found not found!\n");
        return;
    }

    // Create read format.
    const char * format = "(%[^,], %zu)\n";

    // Read loop.
    uint8_t title[255] = {0};
    size_t time = 0;
    size_t entry_index = 1;

    while (1) {
        // Read line.
        size_t entry_members = fscanf(watch_p, format, title, &time);

        if (entry_members == EOF) break;

        // Check for broken entry.
        if (entry_members != 2) {
            printf("Broken entry on line: %zu!\n", entry_index);
            break;
        }

        // Print entry.
        printf("%zu. %s, %zu\n", entry_index, title, time);

        // Increment entry index.
        entry_index++;
    }

    // Return found entries amount. Subtract the last increment
    if (return_entries_found != NULL)
        *return_entries_found = entry_index - 1;

    // Exit good.
    fclose(watch_p);
    return;
}

void add_phase (uint8_t * *user_title)
{
    // Validate user provided title.
    if (user_title == NULL) {
        printf("Invalid user title!\n");
        return;
    }

    // Open file.
    FILE *watch_p = fopen("./watch.txt", "a");
    if (watch_p == NULL) {
        printf("Error creating watch file!\n");
        return;
    }

    // Create write format.
    const char * format = "(%s, %zu)";


    // Write.
    size_t time = 0;
    fprintf(watch_p, format, *user_title, time);

    // Exit good.
    fclose(watch_p);
    return;
}

void remove_phase (size_t * user_choice)
{
    // Validate user choice received.
    if (user_choice == NULL) {
        printf("Invalid user choice!\n");
        return;
    }

    // Open file.
    FILE *watch_p = fopen("./watch.txt", "r+");
    if (watch_p == NULL) {
        printf("Watch file not found!\n");
        return;
    }
    FILE *temp_p = fopen("./temp.txt", "w");
    if (temp_p == NULL) {
        printf("Error creating temp file!\n");
        return;
    }

    // Create formats.
    const char * read_format = "(%[^,], %zu)\n";
    const char * write_format = "(%s, %zu)";

    // Read & copy loop
    uint8_t title[255] = {0};
    size_t time = 0;
    size_t entry_index = 1;

    while (1) {
        // Read watch file.
        size_t entry_members = fscanf(watch_p, read_format, title, &time);

        // Check end of file.
        if (entry_members == EOF) break;

        // Check for broken entry.
        if (entry_members != 2) {
            printf("Broken entry on line: %zu!\n", entry_index);
            break;
        }

        // Write entry to temp file. Entries that don't match the user choice.
        if (entry_index != *user_choice)
            fprintf(temp_p, write_format, title, time);

        // Increment entry index.
        entry_index++;
    }

    // Close file pointers.
    fclose(watch_p);
    fclose(temp_p);

    // Remove old watch file.
    uint8_t error = 0;
    error = remove("./watch.txt");
    if (error) {
        printf("Error renaming watch file!\n");
        return;
    }

    // Rename temp to watch file.
    error = rename("./temp.txt", "./watch.txt");
    if (error) {
        printf("Error renaming temp file to watch file!\n");
        return;
    }
    return;
}

int main(void)
{
    // Create error variable.
    uint8_t error = GOOD;

    // Create menu input.
    size_t * menu_choice = NULL;
    uint8_t * menu_input = NULL;

    // Start loop
    while (1) {
        // Print menu options
        print_menu_options();

        // Get user choice.
        // Repeat if input is invalid. Below 1 or over menu options nr.
        while (1) {
            printf("-> ");
            error = get_user_menu_choice(1, &menu_choice);
            if (error == BAD) {
                printf("Failed to get user menu choice!\n");
                return BAD;
            }
            if (*menu_choice < 1 || *menu_choice > MENU_OPTIONS_NR) {
                printf("Invalid input!\n");
                continue;
            }

            // Valid input.
            break;
        }

        // Direct user to path based on menu choice.
        if (*menu_choice == List) {
            list_phase(NULL);
        }
        else if (*menu_choice == Start) {
            printf("You have chosen the: 'Start' path!\n");
        }
        else if (*menu_choice == Add) {
            // Get user input.
            printf("-> ");
            error = get_user_input(&menu_input);
            if (error == BAD) {
                printf("Failed to get user input!\n");
                return BAD;
            }

            // Pass user inputted title to add phase.
            add_phase(&menu_input);
        }
        else if (*menu_choice == Remove) {
            // Print entries list. Get entries amount.
            size_t entries_found = 0;
            list_phase(&entries_found);

            // Get user menu choice.
            while (1) {
                printf("-> ");
                error = get_user_menu_choice(1, &menu_choice);
                if (error == BAD) {
                    printf("Failed to get user menu choice!\n");
                    return BAD;
                }

                // Check if we got valid entry index.
                if (*menu_choice <= 0 || *menu_choice > entries_found) {
                    printf("Invalid entry index!\n");
                    continue;
                }

                // Valid entry index.
                break;
            }

            printf("User chosed: %zu\n", *menu_choice);
            printf("Entries found: %zu\n", entries_found);

            // Pass user choice to remove phase.
            remove_phase(menu_choice);
        }
    }

    // Exit good.
    free(menu_choice);
    free(menu_input);
    return GOOD;
}
