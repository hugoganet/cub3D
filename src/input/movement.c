/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:20:04 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 15:20:05 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	is_valid_position(t_app *app, double x, double y)
{
	int		map_x;
	int		map_y;
	char	tile;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_y < 0 || map_y >= app->map.height)
		return (false);
	if (!app->map.grid[map_y])
		return (false);
	if (map_x >= (int)ft_strlen(app->map.grid[map_y]))
		return (false);
	tile = app->map.grid[map_y][map_x];
	if (tile == '1')
		return (false);
	return (true);
}

void	rotate_player(t_app *app, int direction)
{
	double	angle;
	double	old_dir_x;
	double	old_plane_x;

	angle = app->rot_speed * direction;
	old_dir_x = app->player.dir.x;
	old_plane_x = app->player.plane.x;
	app->player.dir.x = app->player.dir.x * cos(angle)
		- app->player.dir.y * sin(angle);
	app->player.dir.y = old_dir_x * sin(angle)
		+ app->player.dir.y * cos(angle);
	app->player.plane.x = app->player.plane.x * cos(angle)
		- app->player.plane.y * sin(angle);
	app->player.plane.y = old_plane_x * sin(angle)
		+ app->player.plane.y * cos(angle);
}

void	move_player(t_app *app, double move_x, double move_y)
{
	double	new_x;
	double	new_y;

	new_x = app->player.pos.x + move_x * app->move_speed;
	new_y = app->player.pos.y + move_y * app->move_speed;
	if (is_valid_position(app, new_x, app->player.pos.y))
		app->player.pos.x = new_x;
	if (is_valid_position(app, app->player.pos.x, new_y))
		app->player.pos.y = new_y;
}

static void	apply_movement(t_app *app, double *move_x, double *move_y)
{
	if (app->keys.w)
	{
		*move_x += app->player.dir.x;
		*move_y += app->player.dir.y;
	}
	if (app->keys.s)
	{
		*move_x -= app->player.dir.x;
		*move_y -= app->player.dir.y;
	}
	if (app->keys.a)
	{
		*move_x += app->player.dir.y;
		*move_y -= app->player.dir.x;
	}
	if (app->keys.d)
	{
		*move_x -= app->player.dir.y;
		*move_y += app->player.dir.x;
	}
}

void	update_player_movement(t_app *app)
{
	double	move_x;
	double	move_y;

	move_x = 0;
	move_y = 0;
	apply_movement(app, &move_x, &move_y);
	if (move_x != 0 || move_y != 0)
		move_player(app, move_x, move_y);
	if (app->keys.left)
		rotate_player(app, -1);
	if (app->keys.right)
		rotate_player(app, 1);
}
