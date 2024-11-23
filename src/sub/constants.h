#ifndef CONSTANTS_H
#define CONSTANTS_H

#define DEFAULT_TITLE_CHAR_LIMIT 128
#define WATCH_FILE_PATH "./watch"
#define TEMP_FILE_PATH "./temp"
#define READ_FORMAT "(%[^,], %d)\n"
#define WRITE_FORMAT "(%s, %d)\n"

typedef enum {
    List = 1,
    Add,
    Remove,
    Start,
    menu_options_count
} MENU_OPTIONS;

#endif
