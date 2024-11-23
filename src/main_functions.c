// Disable CRT warnings
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// Entry to Main Functions
#include "main_functions.h"

// Standard C lib
#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_OPTION
#define LIST_OPTION
void list_option(void)
{
    print_and_get_file_options();
}
#endif

#ifndef ADD_OPTION
#define ADD_OPTION
void add_option(void)
{
    FILE *watch_fp = fopen(WATCH_FILE_PATH, "a");
    if (watch_fp == NULL) {
        printf(WATCH_PREFIX);       
        printf(ERROR_CANNOT_CREATE_FILE);
        return;
    }

    u8 *title = NULL;
    while (1) {
        title = get_user_title();
        if (title == NULL) {
            printf(ERROR_INVALID_INPUT);
            continue;
        }
        break;
    }

    fprintf(watch_fp, WRITE_FORMAT, title, 0);

    fclose(watch_fp);
    free(title);
    return;
}
#endif

#ifndef REMOVE_OPTION
#define REMOVE_OPTION
void remove_option(void)
{
    FILE *watch_fp = fopen(WATCH_FILE_PATH, "r");
    if (watch_fp == NULL) {
        printf(WATCH_PREFIX);
        printf(ERROR_FILE_NOT_FOUND);
        return;
    }

    FILE *temp_fp = fopen(TEMP_FILE_PATH, "w");
    if (temp_fp == NULL) {
        printf(TEMP_PREFIX);
        printf(ERROR_CANNOT_CREATE_FILE);
        return;
    }


    u16 watch_title_entries = print_and_get_file_options();
    if (watch_title_entries == 0) {
        printf(WATCH_PREFIX);
        printf(ERROR_EMPTY_FILE);
        return;
    }
    printf("\n");
    u16 user_number = get_user_number(watch_title_entries);


    u8 title[DEFAULT_TITLE_CHAR_LIMIT] = {0};
    u32 time = 0;
    u16 entry = 1;

    while (1) {
        i16 fields = fscanf(watch_fp, READ_FORMAT, title, &time);
        if (fields == EOF) break;

        if (entry == user_number) {
            entry++;
            continue;
        } else {
            fprintf(temp_fp, WRITE_FORMAT, title, time);
            entry++;
        }
    }


    fclose(watch_fp);
    fclose(temp_fp);
    remove(WATCH_FILE_PATH);
    rename(TEMP_FILE_PATH, WATCH_FILE_PATH);

    return;
}
#endif

#ifndef START_OPTION
#define START_OPTION
void start_option(void)
{
    FILE *watch_fp = fopen(WATCH_FILE_PATH, "r");
    if (watch_fp == NULL) {
        printf(WATCH_PREFIX);
        printf(ERROR_FILE_NOT_FOUND);
        return;
    }

    FILE *temp_fp = fopen(TEMP_FILE_PATH, "w");
    if (temp_fp == NULL) {
        printf(TEMP_PREFIX);
        printf(ERROR_CANNOT_CREATE_FILE);
        return;
    }


    u16 watch_title_entries = print_and_get_file_options();
    if (watch_title_entries == 0) {
        printf(WATCH_PREFIX);
        printf(ERROR_EMPTY_FILE);
        return;
    }
    printf("\n");
    u16 user_number = get_user_number(watch_title_entries);


    u8 title[DEFAULT_TITLE_CHAR_LIMIT] = {0};
    u32 time = 0;
    u16 entry = 1;

    while (1) {
        i16 fields = fscanf(watch_fp, READ_FORMAT, title, &time);
        if (fields == EOF) break;

        if (entry == user_number) {
            sleep();
            fprintf(temp_fp, WRITE_FORMAT, title, time + 5);
            printf("%s, %u\n", title, time + 5);
        } else {
            fprintf(temp_fp, WRITE_FORMAT, title, time);
        }

        entry++;
    }


    fclose(watch_fp);
    fclose(temp_fp);
    remove(WATCH_FILE_PATH);
    rename(TEMP_FILE_PATH, WATCH_FILE_PATH);

    return;
}
#endif
