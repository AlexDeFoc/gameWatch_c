#define _CRT_SECURE_NO_WARNINGS

#include <threads.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MENU_OPTIONS 4
#define AUTO_SAVE_MIN 5
#define MAX_TITLE_LENGHT 64

void clear_screen(void);
void pause_screen(void);

void print_menu_options(void);
int get_menu_user_input(const int max_user_input);
char *get_title_user_input(void);
void sleep_thread();

int list_menu_option(void);
void start_menu_option(const int title_user_choice);
void new_menu_option(const char *title);
void remove_menu_option(void);

int main()
{
    enum menu_options {
        List = 1, Start, New, Remove
    };

    char *title = NULL;
    while (1) {
        clear_screen();
        print_menu_options();
        int menu_user_choice = get_menu_user_input(MENU_OPTIONS);

        if (menu_user_choice == List) {
            list_menu_option();
            pause_screen();
        } else if (menu_user_choice == Start) {
            clear_screen();
            int title_count = list_menu_option();
            int title_user_choice = get_menu_user_input(title_count);
            clear_screen();
            while (1) {
                start_menu_option(title_user_choice);
            }
        } else if (menu_user_choice == New) {
            title = get_title_user_input();
            new_menu_option(title);
        } else if (menu_user_choice == Remove) {
            clear_screen();
            remove_menu_option();
        }
    }

    free((void *)title);
    return EXIT_SUCCESS;
}

void clear_screen(void)
{
    printf("\e[1;1H\e[2J");
}

void pause_screen(void)
{
    getc(stdin);
}

void print_menu_options(void)
{
    const char* options[MENU_OPTIONS] = {
        "List",
        "Start",
        "New",
        "Remove"
    };

    for (int i = 0; i < MENU_OPTIONS; i++) {
        printf("%d. %s\n", i+1, options[i]);
    }
}

/* Function returns user_input value got from stdin. */
int get_menu_user_input(const int max_user_input)
{
    int user_input = 0;
    int input_count = 0;

    while (1) {
        printf("-> ");
        input_count = scanf("%d", &user_input);

        if (input_count == EOF) {
            printf("User manually canceled input!\n");
            exit(EXIT_FAILURE);
        } else if (input_count == 0) {
            printf("Invalid input!\n");

            // Flush stdin
            int c = getchar();
            while (c != '\n' && c != EOF) {
                c = getchar();
            }

            continue;
        } else if (user_input < 1 || user_input > max_user_input) {
            printf("Value out of range!\n");
    
            // Flush stdin
            int c = getchar();
            while (c != '\n' && c != EOF) {
                c = getchar();
            }

            continue;
        }

        break;
    }

    // Flush stdin
    int c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }

    return user_input;
}

void sleep_thread()
{
    struct timespec timer;
    timer.tv_sec = AUTO_SAVE_MIN * 2;

    thrd_sleep(&timer, NULL);
}

char *get_title_user_input(void)
{
    char temp_title[MAX_TITLE_LENGHT];

    printf("-> ");
    int c;
    int i = 0;
    while (1) {
        c = fgetc(stdin);
        if (c == '\n' || c == EOF) break;
        temp_title[i] = (char)c;
        i++;
    }

    char *title = malloc(i);

    for (int j = 0; j < i; j++) {
        title[j] = temp_title[j];
    }

    title[i] = '\0';

    return title;
}

int list_menu_option(void)
{
    const char *format = "(%[^,], %d)\n";
    const char *filepath = "./record.txt";
    char title[MAX_TITLE_LENGHT];
    int time;
    FILE *fptr = NULL;

    fptr = fopen(filepath, "r");

    if (fptr == NULL) {
        printf("File doesn't exist!\n");
        return 0;
    }

    int ok = 0;
    int i = 0;
    while (1) {
        ok = fscanf(fptr, format, title, &time);
        if (ok == EOF) {
            break;
        } else if (ok != 2) {
            printf("Broken line: %d, must be (title, time)\n", i+1);
            break;
        };
        printf("%d. (%s, %d)\n", i+1, title, time);
        i = i + 1;
    }
    fclose(fptr);

    return i;
}

void start_menu_option(const int title_user_choice)
{
    sleep_thread();

    const char *read_format = "(%[^,], %d)\n";
    const char *write_format = "(%s, %d)\n";
    const char *temp_filepath = tmpnam(NULL);
    const char *true_filepath = "./record.txt";
    FILE *temp_fptr = NULL;
    FILE *true_fptr = NULL;

    true_fptr = fopen(true_filepath, "r");
    if (true_fptr == NULL) {
        printf("File doesn't exist!\n");
        return;
    }

    temp_fptr = fopen(temp_filepath, "w");
    if (true_fptr == NULL) {
        perror("Error making temp file!\n");
        exit(EXIT_FAILURE);
    }

    /* Copy from true to temp file data
     * and modify the specific entry
    */
    char title[MAX_TITLE_LENGHT] = "\0";
    int time = 0;
    int i = 0;

    int ok = 0;
    while(1) {
        ok = fscanf(true_fptr, read_format, title, &time);

        if (ok == EOF) {
            break;
        } else if (ok != 2) {
            printf("Broken line: %d, must be (title, time)\n", i+1);
            break;
        };

        if (i+1 != title_user_choice) {
            fprintf(temp_fptr, write_format, title, time);
            fflush(temp_fptr);
        } else {
            fprintf(temp_fptr, write_format, title, time + AUTO_SAVE_MIN);
            clear_screen();
            printf("(%s, %d)\n", title, time + AUTO_SAVE_MIN);
        };

        i = i + 1;
    }

    fclose(temp_fptr);
    fclose(true_fptr);
    temp_fptr = NULL;
    true_fptr = NULL;

    remove(true_filepath);
    rename(temp_filepath, true_filepath);
}

void new_menu_option(const char *title)
{
    const char *format = "(%s, %d)\n";
    const char *filepath = "./record.txt";
    FILE *fptr = NULL;

    fptr = fopen(filepath, "a");
    fprintf(fptr, format, title, 0);

    fflush(fptr);
    fclose(fptr);
    fptr = NULL;
}

void remove_menu_option(void)
{
    int title_count = list_menu_option();
    int title_user_choice = get_menu_user_input(title_count);

    const char *read_format = "(%[^,], %d)\n";
    const char *write_format = "(%s, %d)\n";
    const char *temp_filepath = tmpnam(NULL);
    const char *true_filepath = "./record.txt";
    FILE *temp_fptr = NULL;
    FILE *true_fptr = NULL;

    true_fptr = fopen(true_filepath, "r");
    if (true_fptr == NULL) {
        printf("File doesn't exist!\n");
        return;
    }

    temp_fptr = fopen(temp_filepath, "w");
    if (true_fptr == NULL) {
        perror("Error making temp file!\n");
        exit(EXIT_FAILURE);
    }

    // Copy from true to temp file data EXCEPT the line matching the user choice
    char title[MAX_TITLE_LENGHT];
    int time = 0;
    int i = 0;

    int ok = 0;
    while(1) {
        ok = fscanf(true_fptr, read_format, title, &time);

        if (ok == EOF) {
            break;
        } else if (ok != 2) {
            printf("Broken line: %d, must be (title, time)\n", i+1);
            break;
        };
        
        if (i+1 != title_user_choice) {
            fprintf(temp_fptr, write_format, title, time);
            fflush(temp_fptr);
        };

        i = i + 1;
    }

    fclose(temp_fptr);
    fclose(true_fptr);
    temp_fptr = NULL;
    true_fptr = NULL;

    remove(true_filepath);
    rename(temp_filepath, true_filepath);
}
