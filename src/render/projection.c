#include "cub3d.h"
#include <math.h>

double	calculate_wall_height(double perp_dist, int screen_h)
{
	if (perp_dist < 0.001)
		perp_dist = 0.001;
	return (screen_h / perp_dist);
}

void	calculate_wall_bounds(int height, int screen_h, int *draw_start,
		int *draw_end)
{
	*draw_start = -height / 2 + screen_h / 2;
	if (*draw_start < 0)
		*draw_start = 0;
	*draw_end = height / 2 + screen_h / 2;
	if (*draw_end >= screen_h)
		*draw_end = screen_h - 1;
}

void	draw_wall_column(t_app *app, int x, int draw_start, int draw_end,
		int color)
{
	int	y;

	y = draw_start;
	while (y <= draw_end)
	{
		img_put_pixel(&app->frame, x, y, color);
		y++;
	}
}

int	get_texture_coord_x(double wall_x, t_img *texture)
{
	int	tex_x;

	tex_x = (int)(wall_x * (double)texture->w);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->w)
		tex_x = texture->w - 1;
	return (tex_x);
}

static t_img	*get_texture_from_face(t_app *app, int wall_face)
{
	if (wall_face == FACE_NORTH)
		return (&app->tex.north);
	else if (wall_face == FACE_SOUTH)
		return (&app->tex.south);
	else if (wall_face == FACE_EAST)
		return (&app->tex.east);
	else
		return (&app->tex.west);
}

static void	init_texture_vars(t_app *app, t_ray_hit *hit, int draw_start,
		int draw_end, int *vars)
{
	t_img	*texture;
	int		wall_height;
	double	step;
	double	tex_pos;

	texture = get_texture_from_face(app, hit->wall_face);
	vars[0] = get_texture_coord_x(hit->wall_x, texture);
	wall_height = draw_end - draw_start;
	step = (double)texture->h / (double)wall_height;
	tex_pos = (draw_start - app->win_h / 2 + wall_height / 2) * step;
	vars[1] = (int)step;
	vars[2] = (int)tex_pos;
}

void	draw_textured_wall_column(t_app *app, int x, int draw_start,
		int draw_end, t_ray_hit *hit)
{
	t_img	*texture;
	int		vars[3];
	int		y;
	int		tex_y;
	int		color;

	texture = get_texture_from_face(app, hit->wall_face);
	init_texture_vars(app, hit, draw_start, draw_end, vars);
	y = draw_start;
	while (y <= draw_end)
	{
		tex_y = vars[2];
		if (tex_y >= texture->h)
			tex_y = texture->h - 1;
		if (tex_y < 0)
			tex_y = 0;
		color = get_texture_pixel(texture, vars[0], tex_y);
		img_put_pixel(&app->frame, x, y, color);
		vars[2] += vars[1];
		y++;
	}
}

int	get_wall_color(int wall_face)
{
	if (wall_face == FACE_NORTH)
		return (0xFFFFFF);
	else if (wall_face == FACE_SOUTH)
		return (0xAAAAAA);
	else if (wall_face == FACE_EAST)
		return (0x888888);
	else
		return (0x555555);
}
