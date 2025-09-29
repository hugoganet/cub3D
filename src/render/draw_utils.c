#include "cub3d.h"

static void	draw_rect_loop(t_app *app, int *coords, int color)
{
	int	i;
	int	j;

	j = 0;
	while (j < coords[3])
	{
		i = 0;
		while (i < coords[2])
		{
			img_put_pixel(&app->frame, coords[0] + i, coords[1] + j, color);
			i++;
		}
		j++;
	}
}

void	draw_rect(t_app *app, int x, int y, int w, int h, int color)
{
	int	coords[4];

	coords[0] = x;
	coords[1] = y;
	coords[2] = w;
	coords[3] = h;
	draw_rect_loop(app, coords, color);
}

static void	draw_line_step(t_app *app, int *coords, int *deltas, int color)
{
	int	err;
	int	e2;

	err = deltas[0] - deltas[1];
	while (1)
	{
		img_put_pixel(&app->frame, coords[0], coords[1], color);
		if (coords[0] == coords[2] && coords[1] == coords[3])
			break ;
		e2 = 2 * err;
		if (e2 > -deltas[1])
		{
			err -= deltas[1];
			coords[0] += deltas[2];
		}
		if (e2 < deltas[0])
		{
			err += deltas[0];
			coords[1] += deltas[3];
		}
	}
}

static void	init_line_coords(int *coords, int x0, int y0, int x1, int y1)
{
	coords[0] = x0;
	coords[1] = y0;
	coords[2] = x1;
	coords[3] = y1;
}

void	draw_line(t_app *app, int x0, int y0, int x1, int y1, int color)
{
	int	coords[4];
	int	deltas[4];

	init_line_coords(coords, x0, y0, x1, y1);
	deltas[0] = abs(x1 - x0);
	deltas[1] = abs(y1 - y0);
	if (x0 < x1)
		deltas[2] = 1;
	else
		deltas[2] = -1;
	if (y0 < y1)
		deltas[3] = 1;
	else
		deltas[3] = -1;
	draw_line_step(app, coords, deltas, color);
}
