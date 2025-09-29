/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:12 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:13 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>
#include <math.h>
#include <stdlib.h>

static void	rotate_left(t_app *app, double rs)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = app->player.dir.x;
	app->player.dir.x = app->player.dir.x * cos(-rs)
		- app->player.dir.y * sin(-rs);
	app->player.dir.y = old_dir_x * sin(-rs) + app->player.dir.y * cos(-rs);
	old_plane_x = app->player.plane.x;
	app->player.plane.x = app->player.plane.x * cos(-rs)
		- app->player.plane.y * sin(-rs);
	app->player.plane.y = old_plane_x * sin(-rs)
		+ app->player.plane.y * cos(-rs);
}

static void	rotate_right(t_app *app, double rs)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = app->player.dir.x;
	app->player.dir.x = app->player.dir.x * cos(rs)
		- app->player.dir.y * sin(rs);
	app->player.dir.y = old_dir_x * sin(rs) + app->player.dir.y * cos(rs);
	old_plane_x = app->player.plane.x;
	app->player.plane.x = app->player.plane.x * cos(rs)
		- app->player.plane.y * sin(rs);
	app->player.plane.y = old_plane_x * sin(rs) + app->player.plane.y * cos(rs);
}

static void	apply_linear_movement(t_app *app, double ms)
{
	if (app->keys.w)
	{
		app->player.pos.x += app->player.dir.x * ms;
		app->player.pos.y += app->player.dir.y * ms;
	}
	if (app->keys.s)
	{
		app->player.pos.x -= app->player.dir.x * ms;
		app->player.pos.y -= app->player.dir.y * ms;
	}
	if (app->keys.a)
	{
		app->player.pos.x += (-app->player.dir.y) * ms;
		app->player.pos.y += (app->player.dir.x) * ms;
	}
	if (app->keys.d)
	{
		app->player.pos.x -= (-app->player.dir.y) * ms;
		app->player.pos.y -= (app->player.dir.x) * ms;
	}
}

void	apply_movement(t_app *app)
{
	double	ms;
	double	rs;

	ms = app->move_speed;
	rs = app->rot_speed;
	if (app->keys.left)
		rotate_left(app, rs);
	if (app->keys.right)
		rotate_right(app, rs);
	apply_linear_movement(app, ms);
}

int	app_loop(t_app *app)
{
	update_player_movement(app);
	fill_background(app, 0x000000);
	render_background(app);
	render_3d_view(app);
	render_minimap(app);
	mlx_put_image_to_window(app->mlx, app->win, app->frame.ptr, 0, 0);
	return (0);
}

int	close_window(t_app *app)
{
	app_destroy(app, 0);
	exit(0);
	return (0);
}
