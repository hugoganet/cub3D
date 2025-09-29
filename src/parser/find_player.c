/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:47:30 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 15:50:20 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

int	find_player(t_app *app)
{
	int	i;
	int	j;
	int	player_count;

	player_count = 0;
	i = 0;
	while (i < app->map.height)
	{
		j = 0;
		while (app->map.grid[i] && app->map.grid[i][j])
		{
			process_cell_for_player(app, i, j, &player_count);
			j++;
		}
		i++;
	}
	if (player_count == 0)
		error_exit(app, "No player found in map (need N, S, E, or W)");
	if (player_count > 1)
		error_exit(app, "Multiple players found in map (only one allowed)");
	return (0);
}
