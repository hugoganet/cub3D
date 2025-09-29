/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_neighbors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:48:04 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 15:51:40 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

int	is_wall_or_void(t_app *app, int x, int y)
{
	if (y < 0 || y >= app->map.height)
		return (1);
	if (x < 0 || !app->map.grid[y] || x >= (int)ft_strlen(app->map.grid[y]))
		return (1);
	if (app->map.grid[y][x] == '1' || app->map.grid[y][x] == ' ')
		return (1);
	return (0);
}

int	has_open_neighbor(t_app *app, int x, int y)
{
	if (!is_wall_or_void(app, x - 1, y)
		|| !is_wall_or_void(app, x + 1, y)
		|| !is_wall_or_void(app, x, y - 1)
		|| !is_wall_or_void(app, x, y + 1))
		return (1);
	return (0);
}

int	is_at_map_edge(t_app *app, int x, int y)
{
	if (y == 0 || y == app->map.height - 1)
		return (1);
	if (x == 0)
		return (1);
	if (x == (int)ft_strlen(app->map.grid[y]) - 1)
		return (1);
	return (0);
}
