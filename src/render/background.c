#include "cub3d.h"

int	color_to_int(t_color color)
{
	return ((color.r << 16) | (color.g << 8) | color.b);
}

void	draw_ceiling(t_app *app)
{
	int	ceiling_color;
	int	y;
	int	half_height;

	ceiling_color = color_to_int(app->ceil);
	y = 0;
	half_height = DEFAULT_WIN_HEIGHT / 2;
	while (y < half_height)
	{
		draw_rect(app, 0, y, DEFAULT_WIN_WIDTH, 1, ceiling_color);
		y++;
	}
}

void	draw_floor(t_app *app)
{
	int	floor_color;
	int	y;

	floor_color = color_to_int(app->floor);
	y = DEFAULT_WIN_HEIGHT / 2;
	while (y < DEFAULT_WIN_HEIGHT)
	{
		draw_rect(app, 0, y, DEFAULT_WIN_WIDTH, 1, floor_color);
		y++;
	}
}

void	render_background(t_app *app)
{
	draw_ceiling(app);
	draw_floor(app);
}
