// Disable CRT warnings
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// Entry to UTILS
#include "utils.h"

// Standard C lib
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

// UTILS functions
#ifndef UTILS_PRINT_MENU_OPTIONS
#define UTILS_PRINT_MENU_OPTIONS
void print_menu_options(void)
{
#ifdef _WIN32
    const char *options[] = {
        "List",
        "Add",
        "Remove",
        "Start",
        "Stop"
    };
#else
    const char *options[] = {
        "List",
        "Add",
        "Remove",
        "Start"
    };
#endif

    for (int i = 0; i < menu_options_count-1; i++) {
        printf("%d. %s\n", i+1, options[i]);
    }
}
#endif

#ifndef UTILS_PRINT_AND_GET_FILE_OPTIONS
#define UTILS_PRINT_AND_GET_FILE_OPTIONS
u16 print_and_get_file_options(void)
{
    u16 options = 1;
    u8 title[DEFAULT_TITLE_CHAR_LIMIT] = {0};
    u32 time = 0;

    FILE *watch_fp = fopen(WATCH_FILE_PATH, "r");
    if (watch_fp == NULL) {
        printf(WATCH_PREFIX);
        printf(ERROR_FILE_NOT_FOUND);
        return 0;
    }

    printf("\n");
    while (1) {
        i16 fields = fscanf(watch_fp, READ_FORMAT, title, &time);
        if (fields == EOF) {
            break;
        } else if (fields != 2) {
            printf(ERROR_BROKEN_FIELDS, options);
            continue;
        }

        printf("\t%u. %s, %u\n", options, title, time);

        options++;
    }

    fclose(watch_fp);
    return options-1;
}
#endif

#ifndef UTILS_GET_USER_NUMBER
#define UTILS_GET_USER_NUMBER
u16 get_user_number(u16 upper_bound)
{
    i8 c = 0;
    u16 i = 1;
    u16 user_number = 0;

    while (1) {
        user_number = 0;
        i = 1;
        
        printf("-> ");
        while (1) {
            c = getc(stdin);
            if (c == '\n' || c == EOF) break;
            if (c < '0' || c > '9') { // Discard invalid input.
                printf(ERROR_INVALID_INPUT);
                while (getc(stdin) != '\n');
                break;
            }

            // Add next digit.
            if (c != '0') {
                user_number += i * (c - '0');
            } else {
                user_number *= i;
            }

            i *= 10;
        }

        // Check bounds.
        if (user_number <= 0 || user_number > upper_bound) {
            printf(ERROR_OUT_OF_BOUNDS);
            continue;
        }

        break;
    }

    return user_number;
}
#endif

#ifndef UTILS_GET_USER_TITLE
#define UTILS_GET_USER_TITLE
u8 * get_user_title(void)
{
    u32 title_limit = DEFAULT_TITLE_CHAR_LIMIT;
    u8 *title = malloc(title_limit+1);
    if (title == NULL) {
        printf(ERROR_OUT_OF_MEMORY);
        return NULL;
    }
    
    i8 c = 0;
    u32 lenght = 0;
    printf("-> ");
    while (1) {
        c = getc(stdin);
        if (c == '\n' || c == EOF) break;
        if (lenght >= title_limit) {
            u8 *new_title = realloc(title, (title_limit * 2) + 1);
            if (new_title == NULL) {
                printf(ERROR_OUT_OF_MEMORY);
                break;
            }

            title = new_title;
            title_limit *= 2;
        }

        title[lenght] = c;
        lenght++;
    }

    if (lenght != 0) {
        title[lenght] = '\0';
    } else {
        return NULL;
    }

    return title;
}
#endif

#ifndef UTILS_FUNC_SLEEP
#define UTILS_FUNC_SLEEP
void sleep(void)
{
    struct timespec timer = { .tv_sec = 5 * 60 };
    thrd_sleep(&timer, NULL);

    return;
}
#endif
