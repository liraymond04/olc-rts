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

You can run the application using the built executable

```bash
./build/main
```

### Windows

Install a C++ compiler like [MinGW](https://sourceforge.net/projects/mingw/)

Generate project makefiles and build with CMake

```bash
# Use the -d flag to build for debugging
./build.sh
```

### Web build with Emscripten
Web builds use [Emscripten](https://github.com/emscripten-core/emscripten), and need the following additional packages:

```bash
sudo pacman -S --needed emscripten
```

Build by passing in a new target flag to the build script

```bash
## Optionally pass the -d flag to build for debug
./build.sh -t web
```

You can run the application by opening a local web server with emrun

```bash
emrun build/main.html
```
