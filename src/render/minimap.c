#include "cub3d.h"
#include "libft.h"
#include <math.h>

void	draw_minimap_tile(t_app *app, int map_x, int map_y, int color)
{
	int	screen_x;
	int	screen_y;

	screen_x = MINIMAP_OFFSET_X + map_x * MINIMAP_SCALE;
	screen_y = MINIMAP_OFFSET_Y + map_y * MINIMAP_SCALE;
	draw_rect(app, screen_x, screen_y, MINIMAP_SCALE, MINIMAP_SCALE, color);
	draw_rect(app, screen_x, screen_y, MINIMAP_SCALE, 1, COLOR_BORDER);
	draw_rect(app, screen_x, screen_y, 1, MINIMAP_SCALE, COLOR_BORDER);
}

void	draw_player_on_minimap(t_app *app)
{
	int	player_screen_x;
	int	player_screen_y;
	int	dir_end_x;
	int	dir_end_y;

	player_screen_x = MINIMAP_OFFSET_X + (int)(app->player.pos.x
			* MINIMAP_SCALE) - 3;
	player_screen_y = MINIMAP_OFFSET_Y + (int)(app->player.pos.y
			* MINIMAP_SCALE) - 3;
	draw_rect(app, player_screen_x, player_screen_y, 6, 6, COLOR_PLAYER);
	dir_end_x = player_screen_x + 3 + (int)(app->player.dir.x * 10);
	dir_end_y = player_screen_y + 3 + (int)(app->player.dir.y * 10);
	draw_rect(app, dir_end_x, dir_end_y, 2, 2, 0x00FF00);
}

void	render_minimap(t_app *app)
{
	int		x;
	int		y;
	int		line_len;
	char	tile;
	int		color;

	draw_rect(app, MINIMAP_OFFSET_X - 2, MINIMAP_OFFSET_Y - 2,
		app->map.width * MINIMAP_SCALE + 4,
		app->map.height * MINIMAP_SCALE + 4, 0xFFFFFF);
	y = 0;
	while (y < app->map.height)
	{
		x = 0;
		line_len = 0;
		if (app->map.grid[y])
			line_len = ft_strlen(app->map.grid[y]);
		while (x < line_len)
		{
			tile = app->map.grid[y][x];
			if (tile == '1')
				color = 0xFFFFFF;
			else if (tile == '0')
				color = 0x404040;
			else if (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W')
				color = 0x0000FF;
			else if (tile == ' ')
				color = 0x000000;
			else
				color = 0xFF00FF;
			draw_minimap_tile(app, x, y, color);
			x++;
		}
		y++;
	}
	render_minimap_rays(app);
	draw_player_on_minimap(app);
}

void	get_ray_direction(t_app *app, int ray_index, int total_rays,
		t_vec2 *ray_dir)
{
	double	fov_radians;
	double	angle_step;
	double	ray_angle;
	double	player_angle;
	double	final_angle;

	fov_radians = M_PI / 3.0;
	angle_step = fov_radians / (total_rays - 1);
	ray_angle = -fov_radians / 2.0 + ray_index * angle_step;
	player_angle = atan2(app->player.dir.y, app->player.dir.x);
	final_angle = player_angle + ray_angle;
	ray_dir->x = cos(final_angle);
	ray_dir->y = sin(final_angle);
}

void	render_minimap_rays(t_app *app)
{
	int		i;
	t_vec2	ray_dir;
	t_vec2	hit_point;
	int		player_screen_x;
	int		player_screen_y;
	int		hit_screen_x;
	int		hit_screen_y;

	i = 0;
	while (i < MINIMAP_RAY_COUNT)
	{
		get_ray_direction(app, i, MINIMAP_RAY_COUNT, &ray_dir);
		if (cast_minimap_ray(app, ray_dir, &hit_point))
		{
			player_screen_x = MINIMAP_OFFSET_X + (int)(app->player.pos.x
					* MINIMAP_SCALE);
			player_screen_y = MINIMAP_OFFSET_Y + (int)(app->player.pos.y
					* MINIMAP_SCALE);
			hit_screen_x = MINIMAP_OFFSET_X + (int)(hit_point.x
					* MINIMAP_SCALE);
			hit_screen_y = MINIMAP_OFFSET_Y + (int)(hit_point.y
					* MINIMAP_SCALE);
			draw_line(app, player_screen_x, player_screen_y,
				hit_screen_x, hit_screen_y, COLOR_RAY);
		}
		i++;
	}
}
