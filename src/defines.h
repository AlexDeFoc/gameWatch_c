// MAKE CRT GO AWAY:
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

// DEFINE STANDARD C LIBRARIES:
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// DEFINE ENUMS:
#ifndef MENU_OPTIONS_ENUM
#define MENU_OPTIONS_ENUM
typedef enum MENU_OPTIONS {
    MO_LIST = 1, MO_ADD, MO_REMOVE, MO_START
} MENU_OPTIONS;
#endif

// DEFINE TYPES:
// Unsigned types
#ifndef u8
#define u8 uint8_t
#endif
#ifndef u16
#define u16 uint16_t
#endif
#ifndef u32
#define u32 uint32_t
#endif
// Signed types
#ifndef i8
#define i8 int8_t
#endif
#ifndef i16
#define i16 int16_t
#endif
#ifndef i32
#define i32 int32_t
#endif

// DEFINE CONSTANTS:
// File paths
#ifndef WATCH_FILE_PATH
#define WATCH_FILE_PATH "./watch"
#endif
#ifndef TEMP_FILE_PATH
#define TEMP_FILE_PATH "./temp"
#endif
// Read-write formats
#ifndef READ_FORMAT
#define READ_FORMAT "(%[^,], %u)\n"
#endif
#ifndef WRITE_FORMAT
#define WRITE_FORMAT "(%s, %u)\n"
#endif
// General/Other constants
#ifndef DEFAULT_TITLE_LIMIT
#define DEFAULT_TITLE_LIMIT 64
#endif
#ifndef TOTAL_MENU_OPTIONS
#define TOTAL_MENU_OPTIONS 4
#endif

// DEFINE MESSAGES:
// ERRORS:
// Watch file errors
#ifndef M_WATCH_OPEN_AS_READ
#define M_WATCH_OPEN_AS_READ "Watch file: not found!\n"
#endif
#ifndef M_WATCH_OPEN_AS_WRITE
#define M_WATCH_OPEN_AS_WRITE "Watch file: cannot be made/appended to!\n"
#endif
#ifndef M_WATCH_FILE_WRITE
#define M_WATCH_FILE_WRITE "Watch file: error occured when writing to file!\n"
#endif
#ifndef M_WATCH_REMOVE
#define M_WATCH_REMOVE "Watch file: cannot be deleted!\n"
#endif
#ifndef M_WATCH_EMPTY
#define M_WATCH_EMPTY "Watch file: empty file!\n"
#endif
// Temp file errors
#ifndef M_TEMP_OPEN_AS_WRITE
#define M_TEMP_OPEN_AS_WRITE "Temp file: cannot be made!\n"
#endif
#ifndef M_TEMP_FILE_WRITE
#define M_TEMP_FILE_WRITE "Temp file: error occured when writing to file!\n"
#endif
#ifndef M_TEMP_RENAME
#define M_TEMP_RENAME "Temp file: cannot be renamed to watch file!\n"
#endif
// Memory allocation errors
#ifndef M_MALLOC_TITLE
#define M_MALLOC_TITLE "Malloc: not enough memory to allocate the minimum amount of title characters!\n"
#endif
#ifndef M_REALLOC_TITLE
#define M_REALLOC_TITLE "Realloc: not enough memory to enlarge title memory block!\n"
#endif
// File read-write errors
#ifndef M_FILE_BROKEN_GAME_ENTRY
#define M_FILE_BROKEN_GAME_ENTRY "Broken game entry: line %u!\n"
#endif
// Input out of bounds
#ifndef M_OUT_OF_BOUNDS
#define M_OUT_OF_BOUNDS "Input out of bounds!\n"
#endif
