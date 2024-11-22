#include "platform_agnostic.h"

// Utilitary functions
void print_menu_options()
{
    const char *options[] = {
        "List",
        "Add",
        "Remove",
        "Start"
    };

    for (u8 i = 0; i < TOTAL_MENU_OPTIONS; i++) {
        printf("%u. %s\n", i+1, options[i]);
    }

    return;
}

u16 get_user_number(u16 upper_bound)
{
    u16 user_number = 0;
    i8 c = 0;

    while (1) {
        user_number = 0;
        u8 i = 1;

        printf("-> ");
        while (1) {
            c = getc(stdin);
            if (c == '\n' || c == EOF) break;

            if (c < '0' || c > '9') { // Non-digit check.
                while (getc(stdin) != '\n'); // Clear invalid input.
                break;
            }

            if (c != '0') {
                user_number += i * (c-'0'); // Add c as the next digit.
            } else {
                user_number += i * 1; // We got a zero digit.
            }
            i *= 10;
        }

        if (user_number > 0 && user_number <= upper_bound) {
            break;
        }

        printf(M_OUT_OF_BOUNDS); // If we get out of bounds then repeat.
    }

    return user_number;
}

// Main functions
void list_game_titles(void)
{
    // Open watch file in "read-only mode".
    FILE *watch_fp = fopen(WATCH_FILE_PATH, "r");
    if (watch_fp == NULL) {
        printf(M_WATCH_OPEN_AS_READ);
        return;
    }

    // Initialize temp-boxes.
    u32 title_limit = DEFAULT_TITLE_LIMIT;
    u8 *title = malloc(title_limit);
    if (title == NULL) {
        printf(M_MALLOC_TITLE);
        return;
    }
    u32 time = 0;
    u16 game_entry = 1;

    // Start read loop.
    while (1) {
        i8 fields = fscanf(watch_fp, READ_FORMAT, title, &time);
        if (fields == EOF) {
            break;
        } else if (fields != 2) {
            printf(M_FILE_BROKEN_GAME_ENTRY, game_entry);
            fclose(watch_fp);
            free(title);
            return;
        }

        printf("%u. %s, %u\n", game_entry, title, time);
        game_entry++;
    }

    // Clean up & exit.
    fclose(watch_fp);
    free(title);
    return;
}

void add_game_title(void)
{
    // Create/Append watch file in "append mode". Create it non existent.
    FILE *watch_fp = fopen(WATCH_FILE_PATH, "a");
    if (watch_fp == NULL) {
        printf(M_WATCH_OPEN_AS_WRITE);
        return;
    }

    // Initialize temp-boxes.
    u32 title_limit = DEFAULT_TITLE_LIMIT;
    u8 *title = malloc(title_limit + 1);
    if (title == NULL) {
        printf(M_MALLOC_TITLE);
        return;
    }
    i8 c = 0;
    u32 i = 0;

    // Get title loop.
    while (1) {
        c = getc(stdin);
        if (c == '\n' || c == EOF) break;

        // Enlarge title mem block if needed.
        if (i >= title_limit) {
            u8 *new_title_block = realloc(title, (title_limit * 2) + 1);
            if (new_title_block == NULL) {
                printf(M_REALLOC_TITLE);
                break; // Exit loop if we cannot increase title lenght but TRY to still save what we have.
            }

            title = new_title_block;
            title_limit *= 2;
        }

        title[i] = c;
        i++;
    }

    title[i] = '\0'; // Add at end null string terminator char.

    if(fprintf(watch_fp, WRITE_FORMAT, title, 0) < 0) {
        printf(M_WATCH_FILE_WRITE);
        fclose(watch_fp);
        free(title);
        return;
    }

    fclose(watch_fp);
    free(title);
    return;
}

void remove_game_title(void)
{
    // Open watch, temp files.
    FILE *watch_fp = fopen(WATCH_FILE_PATH, "r");
    if (watch_fp == NULL) {
        printf(M_WATCH_OPEN_AS_READ);
        return;
    }
    FILE *temp_fp = fopen(TEMP_FILE_PATH, "w");
    if (temp_fp == NULL) {
        printf(M_TEMP_OPEN_AS_WRITE);
        return;
    }

    // Initialize temp-boxes.
    u32 title_limit = DEFAULT_TITLE_LIMIT;
    u8 *title = malloc(title_limit);
    if (title == NULL) {
        printf(M_MALLOC_TITLE);
        fclose(watch_fp);
        fclose(temp_fp);
        return;
    }
    u32 time = 0;
    u16 game_entry = 0;

    // Find number of entries & list them loop.
    while (1) {
        i16 fields = fscanf(watch_fp, READ_FORMAT, title, &time);
        if (fields == EOF) {
            break;
        } else if (fields != 2) {
            printf(M_FILE_BROKEN_GAME_ENTRY, game_entry);
            fclose(watch_fp);
            fclose(temp_fp);
            free(title);
            return;
        }

        printf("%u. %s, %u\n", game_entry+1, title, time);

        game_entry++;
    }

    if (game_entry == 0) { // Empty file.
        printf(M_WATCH_EMPTY);
        fclose(watch_fp);
        fclose(temp_fp);
        free(title);
        return;
    }

    rewind(watch_fp); // Put file pointer at start of file.

    // Get user to choose entry.
    u16 user_number = get_user_number(game_entry);

    // Setting game entry to 1 because before we had it a nr of entries we found out. And before it started from 0.
    game_entry = 1;

    // Read & write all lines from watch to temp file without selected user number.
    while (1) {
        i16 fields = fscanf(watch_fp, READ_FORMAT, title, &time);
        if (fields == EOF) break;

        if (user_number != game_entry) {
            if (fprintf(temp_fp, WRITE_FORMAT, title, time) < 0) {
                printf(M_TEMP_FILE_WRITE);
                fclose(watch_fp);
                fclose(temp_fp);
                free(title);
                return;
            }
        }

        game_entry++;
    }

    // Clean & rename temp to watch file.
    fclose(watch_fp);
    fclose(temp_fp);
    free(title);
    if (remove(WATCH_FILE_PATH) != 0) {
        printf(M_WATCH_REMOVE);
    }
    if (rename(TEMP_FILE_PATH, WATCH_FILE_PATH) != 0) {
        printf(M_TEMP_RENAME);
    }

    return;
}
