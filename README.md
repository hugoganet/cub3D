# cub3D

A 3D graphics project inspired by Wolfenstein 3D, built using ray-casting techniques.

## About

cub3D is a first-person 3D maze exploration game developed as part of the 42 school curriculum. It implements a ray-casting engine from scratch using the MiniLibX graphics library to render textured walls, handle player movement, and create an immersive 3D environment from a 2D map.

## Features

- **Ray-casting engine** for real-time 3D rendering
- **Textured walls** with different textures for each cardinal direction
- **Player movement** with collision detection
- **Minimap** display with real-time position tracking
- **Customizable maps** via `.cub` configuration files

## Requirements

- C compiler (gcc/clang)
- Make
- MiniLibX library
- X11 development libraries (Linux) or OpenGL (macOS)

## Installation

```bash
git clone https://github.com/hugoganet/cub3D.git
cd cub3D
make
```

## Usage

```bash
./cub3D maps/sample.cub
```

### Controls

- **W/A/S/D** - Move forward/left/backward/right
- **← / →** - Rotate camera
- **ESC** - Exit game

## Map Format

Maps are defined in `.cub` files with:
- Texture paths for walls (NO, SO, WE, EA)
- Floor and ceiling colors (F, C)
- 2D map layout using:
  - `1` - Wall
  - `0` - Empty space
  - `N/S/E/W` - Player spawn position and orientation

## Authors

- Hugo Ganet
- Nicolas Crivelli

*42 School Project*