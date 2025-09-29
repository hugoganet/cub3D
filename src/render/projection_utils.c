/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:30:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 14:30:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

double	calculate_wall_height(double perp_dist, int screen_h)
{
	if (perp_dist < 0.001)
		perp_dist = 0.001;
	return (screen_h / perp_dist);
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
