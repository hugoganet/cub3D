/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:39 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:40 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

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

void	draw_wall_column(t_app *app, int *params, int color)
{
	int	y;

	y = params[1];
	while (y <= params[2])
	{
		img_put_pixel(&app->frame, params[0], y, color);
		y++;
	}
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

static void	init_texture_vars(t_app *app, t_ray_hit *hit, int *bounds,
		int *vars)
{
	t_img	*texture;
	int		wall_height;
	double	step;
	double	tex_pos;

	texture = get_texture_from_face(app, hit->wall_face);
	vars[0] = get_texture_coord_x(hit->wall_x, texture);
	wall_height = bounds[1] - bounds[0];
	step = (double)texture->h / (double)wall_height;
	tex_pos = (bounds[0] - app->win_h / 2 + wall_height / 2) * step;
	vars[1] = (int)step;
	vars[2] = (int)tex_pos;
}

void	draw_textured_wall_column(t_app *app, int *params, t_ray_hit *hit)
{
	t_img	*texture;
	int		vars[3];
	int		bounds[2];
	int		y;
	int		tex_y;

	texture = get_texture_from_face(app, hit->wall_face);
	bounds[0] = params[1];
	bounds[1] = params[2];
	init_texture_vars(app, hit, bounds, vars);
	y = params[1];
	while (y <= params[2])
	{
		tex_y = vars[2];
		if (tex_y >= texture->h)
			tex_y = texture->h - 1;
		if (tex_y < 0)
			tex_y = 0;
		img_put_pixel(&app->frame, params[0], y,
			get_texture_pixel(texture, vars[0], tex_y));
		vars[2] += vars[1];
		y++;
	}
}
