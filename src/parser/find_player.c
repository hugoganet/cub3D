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

/**
 * @brief Recherche et initialise le joueur dans la map.
 *
 * Parcourt toute la grille app->map.grid pour trouver le caractère de
 * joueur (N/S/E/W). Pour chaque cellule, appelle process_cell_for_player()
 * qui incrémente un compteur et configure le joueur si trouvé.
 *
 * Valide qu'il y a exactement un joueur dans la map :
 * - Si aucun joueur : error_exit("No player found")
 * - Si plusieurs joueurs : error_exit("Multiple players found")
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @return int Retourne 0 si exactement un joueur est trouvé.
 *
 */
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
