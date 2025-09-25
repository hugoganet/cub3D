#include "cub3d.h"
#include "libft.h"

int find_player(t_app *app)
{
	int i, j;
	int player_count = 0;

	i = 0;
	while (i < app->map.height)
	{
		j = 0;
		while (app->map.grid[i] && app->map.grid[i][j])
		{
			char c = app->map.grid[i][j];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				player_count++;
				app->player.pos.x = (double)j + 0.5;
				app->player.pos.y = (double)i + 0.5;

				// Set direction and camera plane based on player orientation
				if (c == 'N')
				{
					app->player.dir.x = 0; app->player.dir.y = -1;
					app->player.plane.x = 0.66; app->player.plane.y = 0;
				}
				else if (c == 'S')
				{
					app->player.dir.x = 0; app->player.dir.y = 1;
					app->player.plane.x = -0.66; app->player.plane.y = 0;
				}
				else if (c == 'E')
				{
					app->player.dir.x = 1; app->player.dir.y = 0;
					app->player.plane.x = 0; app->player.plane.y = 0.66;
				}
				else if (c == 'W')
				{
					app->player.dir.x = -1; app->player.dir.y = 0;
					app->player.plane.x = 0; app->player.plane.y = -0.66;
				}
			}
			j++;
		}
		i++;
	}

	if (player_count == 0)
		error_exit(app, "No player found in map (need N, S, E, or W)");

	if (player_count > 1)
		error_exit(app, "Multiple players found in map (only one allowed)");

	return (0);
}

int check_valid_chars(t_app *app)
{
	int i, j;

	i = 0;
	while (i < app->map.height)
	{
		j = 0;
		while (app->map.grid[i] && app->map.grid[i][j])
		{
			char c = app->map.grid[i][j];
			if (c != '0' && c != '1' && c != ' ' &&
				c != 'N' && c != 'S' && c != 'E' && c != 'W')
			{
				printf("Invalid character '%c' at line %d, col %d\n", c, i, j);
				error_exit(app, "Invalid character in map");
			}
			j++;
		}
		i++;
	}

	return (0);
}

int is_wall_or_void(t_app *app, int x, int y)
{
	// Outside map bounds = considered wall
	if (y < 0 || y >= app->map.height)
		return (1);

	if (x < 0 || !app->map.grid[y] || x >= (int)ft_strlen(app->map.grid[y]))
		return (1);

	// Wall or space = OK
	if (app->map.grid[y][x] == '1' || app->map.grid[y][x] == ' ')
		return (1);

	return (0);
}

int check_map_closed(t_app *app)
{
	int i, j;

	i = 0;
	while (i < app->map.height)
	{
		j = 0;
		while (app->map.grid[i] && app->map.grid[i][j])
		{
			char c = app->map.grid[i][j];

			// If it's a floor or player, check if surrounded properly
			if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				// Check all 4 directions
				if (!is_wall_or_void(app, j-1, i) || !is_wall_or_void(app, j+1, i) ||
					!is_wall_or_void(app, j, i-1) || !is_wall_or_void(app, j, i+1))
				{
					// If adjacent to void, check borders
					if (i == 0 || i == app->map.height - 1 || j == 0 ||
						j == (int)ft_strlen(app->map.grid[i]) - 1)
					{
						printf("Map not closed at (%d, %d)\n", j, i);
						error_exit(app, "Map is not properly closed by walls");
					}
				}
			}
			j++;
		}
		i++;
	}

	return (0);
}

int validate_map(t_app *app)
{
	// 1. Check valid characters
	check_valid_chars(app);

	// 2. Find and validate player
	find_player(app);

	// 3. Check if map is closed
	check_map_closed(app);

	return (0);
}
