# Track your in-game time. Auto-saves each 5 minutes.
Benefits:
- Very fast - made in c.
- Safe code - all cases handeled.
- Nice style - auto refreshes terminal aka clears it.
- Titles support spaces.

## Saved file format:
```
(title1, time)
...
(title something smth, time)
```
## Save file name:
The save file is named _"record.txt"_.
It's name resembles a record or a table of names...

## Main menu options:
- List - lists the current available title entries.
- Start - starts the 5 minute timer and the auto-saver.
- New - requests the name of the title and adds a new entry to the save file.
- Remove - removes the entry you select.


## To exit, just force exit it.
Reason: Auto-save goes on forever (the main thread sleeps for the duration of the timer which is 5 minutes).
