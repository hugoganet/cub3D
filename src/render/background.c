/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:27 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:28 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	int	params[4];

	ceiling_color = color_to_int(app->ceil);
	y = 0;
	half_height = DEFAULT_WIN_HEIGHT / 2;
	while (y < half_height)
	{
		params[0] = 0;
		params[1] = y;
		params[2] = DEFAULT_WIN_WIDTH;
		params[3] = 1;
		draw_rect(app, params, ceiling_color);
		y++;
	}
}

void	draw_floor(t_app *app)
{
	int	floor_color;
	int	y;
	int	params[4];

	floor_color = color_to_int(app->floor);
	y = DEFAULT_WIN_HEIGHT / 2;
	while (y < DEFAULT_WIN_HEIGHT)
	{
		params[0] = 0;
		params[1] = y;
		params[2] = DEFAULT_WIN_WIDTH;
		params[3] = 1;
		draw_rect(app, params, floor_color);
		y++;
	}
}

void	render_background(t_app *app)
{
	draw_ceiling(app);
	draw_floor(app);
}
