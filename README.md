# Multiboy

It has long bothered me that there is no Gameboy emulator that seemlessly supports multiplayer on the same system using multiple gamepads. There are some options, but they are not as plug-in-play as I would want them to be. This is where Multiboy comes in.

**Note:** Multiboy is still WIP and is not yet feature complete. Binaries are also not yet available.

This emulator offers the following features:
- Insanely good controller support.
- Each player can play a different game or play the same game.
- Each player has their own saves.

Features that still need to be implemented:
- Link cable support.
- Multi-threading. Right now all players run their emulators on the same thread, causing slowdowns.
- Pause menu.

Special thanks to the mgba community for the libmgba library.

## Building

Before trying to build, make sure SDL2 and SDL2-ttf are available. 


```sh
git clone https://github.com/sharkwouter/multiboy.git --recursive
cd multiboy
mkdir build && cd build
cmake ..
make
```
