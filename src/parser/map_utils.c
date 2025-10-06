/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 05:15:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/06 13:39:24 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Récupère le caractère à une position ou retourne un espace.
 *
 * Permet d'accéder de manière sûre à un caractère de la map.
 * Si la position est hors limites, retourne un espace (' ') qui
 * représente le vide/l'extérieur de la map.
 *
 * Cette fonction est utilisée pour simplifier les vérifications de
 * voisinage sans avoir à tester les limites à chaque fois.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @param x Coordonnée horizontale.
 * @param y Coordonnée verticale.
 * @return char Le caractère à cette position, ou ' ' si hors limites.
 *
 */
int	is_valid_neighbor(t_app *app, int x, int y)
{
	if (y < 0 || y >= app->map.height)
		return (0);
	if (x < 0)
		return (0);
	if (!app->map.grid[y])
		return (0);
	if (x >= (int)ft_strlen(app->map.grid[y]))
		return (0);
	return (1);
}

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
 * Si un caractère invalide est détecté, appelle error_msg() avec
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
			if (c != '0' && c != '1' && c != ' ' && c != 'N'
				&& c != 'S' && c != 'E' && c != 'W')
				return (error_msg("Invalid character in map"));
			j++;
		}
		i++;
	}
	return (0);
}
