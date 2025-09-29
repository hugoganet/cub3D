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
