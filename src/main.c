#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <threads.h>

#define MAIN_OPTIONS_COUNT 4
#define MAX_TITLE_LENGHT 64
#define SLEEP_TIMER_MINS 5

// Utility
void clear_screen();
void hold_screen();

// Entry to main flow.
int list_menu_options();

// Options
void list_option(const char *record_filepath, const char *read_format);
void start_option(const char *record_filepath, const char *temp_filepath, const char *read_format, const char *write_format);
void new_option(const char *record_filepath, const char *write_format);
void remove_option(const char *record_filepath, const char *temp_filepath, const char *read_format, const char *write_format);

int main()
{
    enum menu_options {
        List = 1, Start, New, Remove
    };

    const char *record_filepath = "./record.txt";
    const char *temp_filepath = "./tmp.txt";

    const char *read_format = "(%[^,], %d)\n";
    const char *write_format = "(%s, %d)\n";

    int user_input = 0;
    while (1) {
        user_input = list_menu_options();

        if (user_input == List) {
            list_option(record_filepath, read_format);
        } else if (user_input == Start) {
            start_option(record_filepath, temp_filepath, read_format, write_format);
        } else if (user_input == New) {
            new_option(record_filepath, write_format);
        } else if (user_input == Remove) {
            remove_option(record_filepath, temp_filepath, read_format, write_format);
        }
    }

    return EXIT_SUCCESS;
}

// Clears the screen.
void clear_screen()
{
    printf("\e[1;1H\e[2J");
}

// Pauses the screen and continues after user pressed any key.
void hold_screen()
{
    getchar();
}

// Prints the menu options & gets ui to determine next action.
int list_menu_options()
{
    clear_screen();

    const char *options[MAIN_OPTIONS_COUNT] = {
        "List",
        "Start",
        "New",
        "Remove"
    };

    for (int i = 0; i < MAIN_OPTIONS_COUNT; i++) {
        printf("%d. %s\n", i+1, options[i]);
    }

    int user_input = 0;
    int received_input_values_count = 0;

    while (1) {
        printf("->");
        received_input_values_count = scanf("%d", &user_input);

        if (received_input_values_count == 0) {
            printf("Invalid input!\n");

            int c = getchar();
            while (c != '\n' && c != EOF) c = getchar();

            continue;
        } else if (user_input < 1 || user_input > MAIN_OPTIONS_COUNT) {
            printf("Out of range!\n");

            int c = getchar();
            while (c != '\n' && c != EOF) c = getchar();

            continue;
        }

        break;
    }

    int c = getchar();
    while (c != '\n' && c != EOF) c = getchar();

    return user_input;
}

// Lists entries from record file.
void list_option(const char *record_filepath, const char *read_format)
{
    clear_screen();

    FILE *record_fptr = fopen(record_filepath, "r");

    if (record_fptr == NULL) {
        printf("Record file not found!\n");
        hold_screen();
        return;
    }

    char title[MAX_TITLE_LENGHT+1];
    int time;

    int received_fields_count = 0;
    int title_index = 0;
    while (1) {
        received_fields_count = fscanf(record_fptr, read_format, title, &time);

        if (received_fields_count == EOF) {
            break;
        } else if (received_fields_count != 2) {
            printf("Line: %d broken\n Must be (title, time)\n", title_index + 1);
            hold_screen();
            exit(EXIT_FAILURE);
        }

        printf("%d. %s - %d minutes\n", title_index + 1, title, time);

        title_index = title_index + 1;
    }

    fclose(record_fptr);
    record_fptr = NULL;

    hold_screen();
}

// Starts a 5 minutes timer and modifies the time value of the selected time, indefinitely.
void start_option(const char *record_filepath, const char *temp_filepath, const char *read_format, const char *write_format)
{
    clear_screen();

    // PRINT TITLES
    FILE *record_fptr = fopen(record_filepath, "r");
    if (record_fptr == NULL) {
        printf("Record file not found!\n");
        hold_screen();
        return;
    }

    char title[MAX_TITLE_LENGHT+1];
    int time = 0;
    int received_fields_count = 0;
    int title_index = 0;
    while (1) {
        received_fields_count = fscanf(record_fptr, read_format, title, &time);

        if (received_fields_count == EOF) {
            break;
        } else if (received_fields_count != 2) {
            printf("Line: %d broken\n Must be (title, time)\n", title_index + 1);
            hold_screen();
            exit(EXIT_FAILURE);
        }

        printf("%d. %s - %d minutes\n", title_index + 1, title, time);

        title_index = title_index + 1;
    }
    fclose(record_fptr);
    record_fptr = NULL;


    // GET UI
    printf("\nWhich title to start?\n");
    
    int user_input = 0;
    int received_input_values_count = 0;
    while (1) {
        printf("->");
        received_input_values_count = scanf("%d", &user_input);

        if (received_input_values_count == 0) {
            printf("Invalid input!\n");

            int c = getchar();
            while (c != '\n' && c != EOF) c = getchar();

            continue;
        } else if (user_input < 1 || user_input > title_index) {
            printf("Out of range!\n");

            int c = getchar();
            while (c != '\n' && c != EOF) c = getchar();

            continue;
        }

        break;
    }

    int c = getchar();
    while (c != '\n' && c != EOF) c = getchar();

    // Sleep, read, write/update, remove, rename, loop.
    struct timespec timer;
    timer.tv_sec = SLEEP_TIMER_MINS * 60;
    
    FILE *temp_fptr = NULL;
    while (1) {
        title_index = 0;
        thrd_sleep(&timer, NULL);

        record_fptr = fopen(record_filepath, "r");
        temp_fptr = fopen(temp_filepath, "w");

        while (1) {
            received_fields_count = fscanf(record_fptr, read_format, title, &time);

            if (received_fields_count == EOF) break;

            if (title_index == user_input - 1) {
                fprintf(temp_fptr, write_format, title, time + SLEEP_TIMER_MINS);
                fflush(temp_fptr);
                clear_screen();
                printf("(%s, %d minutes)\n", title, time + SLEEP_TIMER_MINS);
            } else {
                fprintf(temp_fptr, write_format, title, time);
                fflush(temp_fptr);
            }

            title_index = title_index + 1;
        }

        fclose(record_fptr);
        fclose(temp_fptr);
        record_fptr = NULL;
        temp_fptr = NULL;

        remove(record_filepath);
        rename(temp_filepath, record_filepath);
    }
}

// Appends new entry with user inputed title & time 0 into record file.
void new_option(const char *record_filepath, const char *write_format)
{
    clear_screen();

    // Get title phase.
    char dump[MAX_TITLE_LENGHT+1];
    printf("Insert title name\n");
    printf("-> ");
    
    int char_read;
    int title_lenght = 0;
    while (1) {
        char_read = fgetc(stdin);
        if (char_read == '\n' || char_read == EOF || title_lenght >= 64) break;
        dump[title_lenght] = (char)char_read;
        title_lenght++;
    }

    char *title = malloc(title_lenght);
    for (int j = 0; j < title_lenght; j++) {
        title[j] = dump[j];
    }

    title[title_lenght] = '\0';

    // Open & write to file new entry.
    FILE *record_fptr = fopen(record_filepath, "a");
    fprintf(record_fptr, write_format, title, 0);
    fflush(record_fptr);
    fclose(record_fptr);
    record_fptr = NULL;
}

// Removes a entry from the record file.
void remove_option(const char *record_filepath, const char *temp_filepath, const char *read_format, const char *write_format)
{
    clear_screen();

    // PRINT TITLES
    FILE *record_fptr = fopen(record_filepath, "r");
    if (record_fptr == NULL) {
        printf("Record file not found!\n");
        hold_screen();
        return;
    }

    char title[MAX_TITLE_LENGHT+1];
    int time = 0;
    int received_fields_count = 0;
    int title_index = 0;
    while (1) {
        received_fields_count = fscanf(record_fptr, read_format, title, &time);

        if (received_fields_count == EOF) {
            break;
        } else if (received_fields_count != 2) {
            printf("Line: %d broken\n Must be (title, time)\n", title_index + 1);
            hold_screen();
            exit(EXIT_FAILURE);
        }

        printf("%d. %s - %d minutes\n", title_index + 1, title, time);

        title_index = title_index + 1;
    }
    rewind(record_fptr);


    // GET UI
    printf("\nWhich title to delete?\n");
    
    int user_input = 0;
    int received_input_values_count = 0;
    while (1) {
        printf("->");
        received_input_values_count = scanf("%d", &user_input);

        if (received_input_values_count == 0) {
            printf("Invalid input!\n");

            int c = getchar();
            while (c != '\n' && c != EOF) c = getchar();

            continue;
        } else if (user_input < 1 || user_input > title_index) {
            printf("Out of range!\n");

            int c = getchar();
            while (c != '\n' && c != EOF) c = getchar();

            continue;
        }

        break;
    }

    int c = getchar();
    while (c != '\n' && c != EOF) c = getchar();


    // READ, COPY & REMOVE SELECTED ENTRY.
    FILE *temp_fptr = fopen(temp_filepath, "w");

    title_index = 0;
    while (1) {
        received_fields_count = fscanf(record_fptr, read_format, title, &time);
        if (received_fields_count == EOF) {
            break;
        }
        
        if (title_index != user_input - 1) {
            fprintf(temp_fptr, write_format, title, time);
            fflush(temp_fptr);
        }

        title_index = title_index + 1;
    }
    fclose(record_fptr);
    fclose(temp_fptr);
    record_fptr = NULL;
    temp_fptr = NULL;
    remove(record_filepath);
    rename(temp_filepath, record_filepath);
}
