/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 05:15:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/03 14:55:02 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Détermine si un caractère représente une cellule traversable.
 *
 * Une cellule est traversable si elle correspond à :
 * - Un espace vide ('0')
 * - Une position de départ du joueur ('N', 'S', 'E', 'W')
 *
 * Utilisé pour identifier les cellules qui nécessitent une vérification
 * de fermeture (doivent être entourées de murs ou d'autres cellules valides).
 *
 * @param c Le caractère à tester.
 * @return int 1 si le caractère est traversable, 0 sinon.
 *
 */
int	is_walkable_char(char c)
{
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

/**
 * @brief Vérifie si une position est dans les limites de la map.
 *
 * Valide qu'une coordonnée (x,y) se trouve à l'intérieur des bornes
 * de la grille de la map. Vérifie :
 * - Que y est positif et inférieur à la hauteur totale
 * - Que x est positif
 * - Que la ligne existe dans la grille
 * - Que x est inférieur à la longueur de la ligne
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @param x Coordonnée horizontale (colonne).
 * @param y Coordonnée verticale (ligne).
 * @return int 1 si la position est dans les limites, 0 sinon.
 *
 */
int	in_bounds(t_app *app, int x, int y)
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
char	get_map_char_or_space(t_app *app, int x, int y)
{
	if (!in_bounds(app, x, y))
		return (' ');
	return (app->map.grid[y][x]);
}
