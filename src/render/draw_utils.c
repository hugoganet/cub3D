/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:32 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:33 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	draw_rect(t_app *app, int *params, int color)
{
	int	coords[4];

	coords[0] = params[0];
	coords[1] = params[1];
	coords[2] = params[2];
	coords[3] = params[3];
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

void	draw_line(t_app *app, int *params, int color)
{
	int	coords[4];
	int	deltas[4];

	coords[0] = params[0];
	coords[1] = params[1];
	coords[2] = params[2];
	coords[3] = params[3];
	deltas[0] = abs(params[2] - params[0]);
	deltas[1] = abs(params[3] - params[1]);
	if (params[0] < params[2])
		deltas[2] = 1;
	else
		deltas[2] = -1;
	if (params[1] < params[3])
		deltas[3] = 1;
	else
		deltas[3] = -1;
	draw_line_step(app, coords, deltas, color);
}
