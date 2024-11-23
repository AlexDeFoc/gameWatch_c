# gameWatch - track your in-game time easily.

## Storage
gameWatch stores it's tracked time along side it's title part of the entry.
The file is called "watch".
Example of contents:
```
(Stardew Valley, 100)
(Among Us, 321)
(Undertale, 10000)
(Minecraft, 31231)
```
The application stores minutes.

## Auto-save
The main reason of the creation for this app is in the case of a power outtage, your time spent is saved.
The auto-save feature is called every 5 minutes.

## Limits
The app has a limit for the minutes it can hold. It's 2^32 minutes. Which is about 8185 years. ;)
But if you put negative minutes into the watch file idk what happends.

## How to exit
The app will infinitely do this after you choose the start option. Therefore to exit you need to do it manually.