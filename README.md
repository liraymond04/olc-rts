# olc-rts

Simple pixel-style hex based RTS game.

Project using javidx9's [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)

![Untitled](https://github.com/liraymond04/olc-rts/assets/39678448/3e727835-1750-4ab7-89ab-e193114c25bc)

## Controls

| Button | Action |
| ----------- | ----------- |
| W | Move camera up |
| A | Move camera left |
| S | Move camera right |
| D | Move camera down |
| Z | Increase hex tile height |
| C | Decrease hex tile height |
| Left click | Select hex tile |
| Right click | Move player (if selected hex tile has player) |

## Building

Building is based on Moros1138's [pge-template-project](https://github.com/Moros1138/pge-template-project) CMake files

### Arch Linux

Install the required packages with the following command

`sudo pacman -Sy base-devel cmake git libpng mesa`

Generate project makefiles and build with CMake

```bash
# Use the -d flag to build for debugging
./build.sh
```

### Windows

Install a C++ compiler like [MinGW](https://sourceforge.net/projects/mingw/)

Generate project makefiles and build with CMake

```bash
# Use the -d flag to build for debugging
./build.sh
```
