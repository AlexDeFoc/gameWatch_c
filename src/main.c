#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

#define MENU_OPTIONS 4
#define AUTO_SAVE_MINUTES 5
#define MAX_TITLE_LENGHT 50

void print_menu_options() {
    char *menu_items[MENU_OPTIONS] = {
        "List",
        "Start",
        "New",
        "Remove"
    };

    for (int i = 0; i < MENU_OPTIONS; i++) {
        printf("%d. %s\n", i+1, menu_items[i]);
    }

    printf("-> ");
}

void get_user_menu_input(int *container) {
    int ch;
    while (1) {
        ch = getc(stdin);

        while (getc(stdin) != '\n'); // flush

        if (ch >= '1' && ch <= MENU_OPTIONS + '0') {
            *container = ch - '0'; // convert ascii to int
            break;
        } else {
            printf("Invalid input!\n");
        }
    }
}

void get_title_name(char **name_buf, int *lenght) {
    printf("Enter name: ");

    int ch = 0;
    int i = 0;
    while (1) {
        ch = getc(stdin);
        if (ch < 0 || ch == '\n') break;

        (*name_buf)[i] = (char)ch;
        i++;
    }

    (*name_buf)[i] = '\0';
    *lenght = i;
}

int main() {
    char title_buffer[MAX_TITLE_LENGHT] = {0};
    int time_buffer = 0;

    enum menu_opts {
        List = 1, Start, Stop, New, Remove
    };

    const char *record_path = "./record.txt";
    const char *temp_path = "./temp.txt";

    FILE *record = NULL;
    FILE *temp = NULL;

    const char *ITEM_FORMAT_EXPORT = "(%s, %d)\n";
    const char *ITEM_FORMAT_IMPORT = "(%[^,], %d)\n";

    int menu_ui = 0;
    char *name_buffer = malloc(MAX_TITLE_LENGHT * sizeof(char));
    int name_lenght = 0;
    if (name_buffer == NULL) {
        printf("No memory!\n");
        return 1;
    }

    // Start
    print_menu_options();
    get_user_menu_input(&menu_ui);

    if (menu_ui == List) {
        fopen_s(&record, record_path, "r");

        if (record == NULL) {
            printf("No record file found!\n");
            return 1;
        }

        int i = 1;
        while (1) {
            int ok = fscanf_s(record, ITEM_FORMAT_IMPORT, title_buffer, MAX_TITLE_LENGHT, &time_buffer);

            if (ok == EOF) break;

            if (ok != 2) {
                printf("Malformed record file!\n");
                fclose(record);
                return 1;
            }

            printf("%d. %s - %d minutes\n", i, title_buffer, time_buffer);
            i++;
        }
        fseek(record, 0, SEEK_SET);
        fclose(record);
    } else if (menu_ui == New) {
        get_title_name(&name_buffer, &name_lenght);
        fopen_s(&record, record_path, "a");

        fprintf_s(record, ITEM_FORMAT_EXPORT, name_buffer, 0);
        fseek(record, 0, SEEK_SET);

        fclose(record);
    } else if (menu_ui == Remove) {
        // Print items list
        fopen_s(&record, record_path, "r");

        if (record == NULL) {
            printf("Failted to open file!\n");
            return 1;
        }

        int i = 0;
        while (1) {
            int ok = fscanf_s(record, ITEM_FORMAT_IMPORT, title_buffer, MAX_TITLE_LENGHT, &time_buffer);

            if (ok == EOF) break;

            if (ok != 2) {
                printf("Malformed file!\n");
                fclose(record);
                return 1;
            }

            i++;
            printf("%d. %s - %d minutes\n", i, title_buffer, time_buffer);
        }
        fclose(record);

        // Get ui
        printf("-> ");
        get_user_menu_input(&menu_ui);

        fopen_s(&record, record_path, "r");
        fopen_s(&temp, temp_path, "w");

        if (record == NULL || temp == NULL) {
            printf("Failted to open files!\n");
            return 1;
        }

        i = 0;
        while (1) {
            int ok = fscanf_s( record, ITEM_FORMAT_IMPORT, title_buffer, MAX_TITLE_LENGHT, &time_buffer);

            if (ok == EOF) break;

            i++;
            if (i == menu_ui) {
                continue;
            }

            fprintf_s(temp, ITEM_FORMAT_EXPORT, title_buffer, time_buffer);
        }

        // Close both files
        fclose(record);
        fclose(temp);

        remove(record_path);
        rename(temp_path, record_path);
    } else if (menu_ui == Start) {
        // Print items list
        fopen_s(&record, record_path, "r");

        if (record == NULL) {
            printf("Failted to open file!\n");
            free(name_buffer);
            return 1;
        }

        int i = 0;
        while (1) {
            int ok = fscanf_s(
                record,
                ITEM_FORMAT_IMPORT,
                title_buffer,
                MAX_TITLE_LENGHT,
                &time_buffer
            );

            if (ok == EOF) break;

            if (ok != 2) {
                printf("Malformed file!\n");
                fclose(record);
                free(name_buffer);
                return 1;
            }

            i++;
            printf("%d. %s - %d minutes\n", i, title_buffer, time_buffer);
        }
        fclose(record);

        // Get ui
        printf("-> ");
        get_user_menu_input(&menu_ui);

        // Auto-save timer/watch/clock - every 5 minutes
        fopen_s(&record, record_path, "r+");
        if (record == NULL) {
            printf("Failed to open file for uploading!\n");
            free(name_buffer);
            return 1;
        }

        while (1) {
            // Reset file pointer
            rewind(record);
            i = 0;

            while (1) {
                long line_pos = ftell(record);
                fscanf_s( record, ITEM_FORMAT_IMPORT, title_buffer, MAX_TITLE_LENGHT, &time_buffer);

                i++;
                if (menu_ui == i) {
                    thrd_sleep(&(struct timespec){.tv_sec=(60 * AUTO_SAVE_MINUTES)}, NULL);
                    time_buffer += AUTO_SAVE_MINUTES;

                    fseek(record, line_pos, SEEK_SET);
                    fprintf_s(record, ITEM_FORMAT_EXPORT, title_buffer, time_buffer);
                    fflush(record);     // Write immediately to file

                    printf("%s - %d minutes\n", title_buffer, time_buffer);
                    break;
                }
            }
        }

        fclose(record);
    }

    return 0;
}
