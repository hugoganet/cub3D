/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_chars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:47:45 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 15:50:59 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

/**
 * @brief Vérifie que tous les caractères de la map sont valides.
 *
 * Parcourt chaque cellule de la grille app->map.grid et valide que
 * seuls les caractères autorisés sont présents :
 * - '0' : espace vide (sol)
 * - '1' : mur
 * - ' ' : espace (vide non traversable)
 * - 'N', 'S', 'E', 'W' : position et orientation du joueur
 *
 * Si un caractère invalide est détecté, appelle error_exit() avec
 * un message explicite.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @return int Retourne 0 si tous les caractères sont valides.
 *
 */
int	check_valid_chars(t_app *app)
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
			if (c != '0' && c != '1' && c != ' '
				&& c != 'N' && c != 'S' && c != 'E' && c != 'W')
				error_exit(app, "Invalid character in map");
			j++;
		}
		i++;
	}
	return (0);
}
