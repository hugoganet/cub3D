# cub3D Skeleton

This is a Norm-friendly skeleton for the 42 `cub3D` project on macOS using MiniLibX and your existing `libft`.

Status: minimal loop, background rendering, input flags; parser and raycasting are stubbed. Build verifies project structure and linkage.

## Build

Requirements:
- macOS
- MiniLibX installed (lib at `/usr/local/lib`, headers at `/usr/local/include`) or set `MLX_DIR` to your local MLX root
- `libft/` in this repo (already present)

Build:

```
make
```

Run:

```
./cub3D maps/sample.cub
```

If you see MLX link errors, set `MLX_DIR`:

```
make MLX_DIR=/opt/homebrew
```

## Structure

- `includes/` headers
- `src/` with submodules: `render`, `parser`, `input`, `utils`
- `assets/textures/` for XPM textures
- `maps/` example map

## Next steps

- Implement `.cub` parsing (headers for textures/colors, then map), with validation
- Implement raycasting (camera plane + DDA) and textured walls
- Add collision using the parsed map

