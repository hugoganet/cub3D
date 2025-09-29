#include "cub3d.h"
#include <mlx.h>
#include <stdlib.h>

static void	clear_frame(t_img *img, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < img->h)
	{
		x = 0;
		while (x < img->w)
		{
			img_put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

void	render_frame(t_app *app)
{
	int	floor_color;
	int	y;
	int	x;

	clear_frame(&app->frame, rgb_to_int(app->ceil));
	floor_color = rgb_to_int(app->floor);
	y = app->frame.h / 2;
	while (y < app->frame.h)
	{
		x = 0;
		while (x < app->frame.w)
		{
			img_put_pixel(&app->frame, x, y, floor_color);
			x++;
		}
		y++;
	}
}

void	img_put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= 1024 || y >= 768)
		return ;
	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

void	fill_background(t_app *app, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < app->win_h)
	{
		x = 0;
		while (x < app->win_w)
		{
			img_put_pixel(&app->frame, x, y, color);
			x++;
		}
		y++;
	}
}
