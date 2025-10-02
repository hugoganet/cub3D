/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:33:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:33:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

int	load_textures(t_app *app)
{
	if (load_single_texture(app, app->tex.north_path, &app->tex.north))
		return (1);
	if (load_single_texture(app, app->tex.south_path, &app->tex.south))
	{
		free_textures(app);
		return (1);
	}
	if (load_single_texture(app, app->tex.west_path, &app->tex.west))
	{
		free_textures(app);
		return (1);
	}
	if (load_single_texture(app, app->tex.east_path, &app->tex.east))
	{
		free_textures(app);
		return (1);
	}
	app->tex.loaded = true;
	return (0);
}

t_img	*get_wall_texture(t_app *app, int side, t_vec2 ray_dir)
{
	if (side == 0)
	{
		if (ray_dir.x > 0)
			return (&app->tex.east);
		else
			return (&app->tex.west);
	}
	else
	{
		if (ray_dir.y > 0)
			return (&app->tex.south);
		else
			return (&app->tex.north);
	}
}
