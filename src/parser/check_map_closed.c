/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_closed.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:48:23 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 15:51:53 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

/**
 * @brief Vérifie que la carte est correctement fermée par des murs.
 *
 * Parcourt chaque cellule de la grille app->map.grid. Pour chaque cellule
 * contenant un caractère de joueur (N/S/E/W), vérifie que la cellule
 * n'a pas de voisin "ouvert" (i.e. non mur/non espace) et qu'elle n'est
 * pas située sur le bord de la carte. Si une cellule de joueur enfreint
 * ces règles, appelle error_exit() avec un message explicite.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @return int Toujours 0 si la vérification complète (error_exit termine
 *             le programme en cas d'erreur).
 */
int	check_map_closed(t_app *app)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < app->map.height)
	{
		j = 0;
		while (app->map.grid[i] && app->map.grid[i][j])
		{
			c = app->map.grid[i][j];
			if (is_player_char(c))
			{
				if (has_open_neighbor(app, j, i) && is_at_map_edge(app, j, i))
					error_exit(app, "Map is not properly closed by walls");
			}
			j++;
		}
		i++;
	}
	return (0);
}

int	validate_map(t_app *app)
{
	check_valid_chars(app);
	find_player(app);
	check_map_closed(app);
	return (0);
}
