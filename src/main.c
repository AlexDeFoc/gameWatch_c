#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <threads.h>

struct item {
    char name[50];
    int time;
};

void print_menu_items() {
    printf("1. List\n");
    printf("2. Start\n");
    printf("3. New\n");
    printf("4. Remove\n");
    printf("-> ");
}

void get_user_menu_input(int *container, int max) {
    int ch;
    while (1) {
        ch = getc(stdin);

        while (getc(stdin) != '\n'); // flush

        if (ch >= '1' && ch <= max + '0') {
            *container = ch - '0'; // convert ascii to int
            break;
        } else {
            printf("Invalid input!\n");
        }
    }
}

void new_menu(char **name_buf, int *lenght) {
    printf("Enter name: ");

    int ch;
    int i = 0;
    while (1) {
        ch = getc(stdin);
        if (ch == '\n' || ch < 0) break;

        (*name_buf)[i] = (char)ch;
        i++;
    }

    (*name_buf)[i] = '\0';
    *lenght = i;
}

int main() {
    struct item dummy_item = {
        .name = "",
        .time = 0
    };

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
    char *name_buffer = malloc(50 * sizeof(char));
    int name_lenght = 0;
    if (name_buffer == NULL) {
        printf("No memory!\n");
        return 1;
    }

    // Start
    print_menu_items();
    get_user_menu_input(&menu_ui, 4); // Menu options

    if (menu_ui == List) {
        fopen_s(&record, record_path, "r");

        if (record == NULL) {
            printf("Failed to open file!\n");
            return 1;
        }

        int i = 0;
        while (1) {
            int ok = fscanf_s(
                    record,
                    ITEM_FORMAT_IMPORT,
                    dummy_item.name,
                    50,
                    &dummy_item.time
                    );

            if (ok == EOF) break;

            if (ok != 2) {
                printf("Malformed file!\n");
                fclose(record);
                return 1;
            }

            i++;
            printf("%d. %s - %d minutes\n", i, dummy_item.name, dummy_item.time);
        }
        fseek(record, 0, SEEK_SET);
        fclose(record);
    } else if (menu_ui == New) {
        new_menu(&name_buffer, &name_lenght);
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
            int ok = fscanf_s(
                    record,
                    ITEM_FORMAT_IMPORT,
                    dummy_item.name,
                    50,
                    &dummy_item.time
                    );

            if (ok == EOF) break;

            if (ok != 2) {
                printf("Malformed file!\n");
                fclose(record);
                return 1;
            }

            i++;
            printf("%d. %s - %d minutes\n", i, dummy_item.name, dummy_item.time);
        }
        fclose(record);

        // Get ui
        printf("-> ");
        get_user_menu_input(&menu_ui, i);

        fopen_s(&record, record_path, "r");
        fopen_s(&temp, temp_path, "w");

        if (record == NULL || temp == NULL) {
            printf("Failted to open files!\n");
            return 1;
        }

        i = 0;
        while (1) {
            int ok = fscanf_s(
                    record,
                    ITEM_FORMAT_IMPORT,
                    dummy_item.name,
                    50,
                    &dummy_item.time
                    );

            if (ok == EOF) break;

            i++;
            if (i == menu_ui) {
                continue;
            }

            fprintf_s(temp, ITEM_FORMAT_EXPORT, dummy_item.name, dummy_item.time);
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
                    dummy_item.name,
                    50,
                    &dummy_item.time
                    );

            if (ok == EOF) break;

            if (ok != 2) {
                printf("Malformed file!\n");
                fclose(record);
                free(name_buffer);
                return 1;
            }

            i++;
            printf("%d. %s - %d minutes\n", i, dummy_item.name, dummy_item.time);
        }
        fclose(record);

        // Get ui
        printf("-> ");
        get_user_menu_input(&menu_ui, i);

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
                fscanf_s(
                        record,
                        ITEM_FORMAT_IMPORT,
                        dummy_item.name,
                        50,
                        &dummy_item.time
                        );

                i++;
                if (menu_ui == i) {
                    thrd_sleep(&(struct timespec){.tv_sec=(60 * 5)}, NULL);
                    dummy_item.time += 5;
                    printf("%s - %d minutes\n", dummy_item.name, dummy_item.time);
                    fseek(record, line_pos, SEEK_SET);
                    fprintf_s(record, ITEM_FORMAT_EXPORT, dummy_item.name, dummy_item.time);
                    fflush(record); // Ensure that the data is written immediately to file
                    break;
                }
            }
        }

        fclose(record);
    }

    return 0;
}
