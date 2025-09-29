/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:42 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:43 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	cast_minimap_ray(t_app *app, t_vec2 ray_dir, t_vec2 *hit_point)
{
	t_ray_hit	hit;

	if (!cast_ray(app, ray_dir, &hit))
		return (0);
	hit_point->x = app->player.pos.x + hit.perp_dist * ray_dir.x;
	hit_point->y = app->player.pos.y + hit.perp_dist * ray_dir.y;
	return (1);
}
