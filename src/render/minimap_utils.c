/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 14:00:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include <math.h>

int	get_tile_color(char tile)
{
	if (tile == '1')
		return (0xFFFFFF);
	else if (tile == '0')
		return (0x404040);
	else if (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W')
		return (0x0000FF);
	else if (tile == ' ')
		return (0x000000);
	else
		return (0xFF00FF);
}

void	draw_minimap_border(t_app *app)
{
	int	border_params[4];

	border_params[0] = MINIMAP_OFFSET_X - 2;
	border_params[1] = MINIMAP_OFFSET_Y - 2;
	border_params[2] = app->map.width * MINIMAP_SCALE + 4;
	border_params[3] = app->map.height * MINIMAP_SCALE + 4;
	draw_rect(app, border_params, 0xFFFFFF);
}

void	draw_minimap_ray(t_app *app, t_vec2 hit_point)
{
	int	line_params[4];

	line_params[0] = MINIMAP_OFFSET_X + (int)(app->player.pos.x
			* MINIMAP_SCALE);
	line_params[1] = MINIMAP_OFFSET_Y + (int)(app->player.pos.y
			* MINIMAP_SCALE);
	line_params[2] = MINIMAP_OFFSET_X + (int)(hit_point.x * MINIMAP_SCALE);
	line_params[3] = MINIMAP_OFFSET_Y + (int)(hit_point.y * MINIMAP_SCALE);
	draw_line(app, line_params, COLOR_RAY);
}
