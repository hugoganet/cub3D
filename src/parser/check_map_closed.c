/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map_closed.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:48:23 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/06 16:44:05 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Vérifie que les 4 voisins d'une cellule ne sont pas du vide.
 *
 * Pour une position donnée (x,y), examine les quatre cellules adjacentes
 * (gauche, droite, haut, bas) et s'assure qu'aucune n'est un espace ' '.
 * Un espace représente le vide/l'extérieur et invalide la fermeture.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @param x Coordonnée horizontale de la cellule à vérifier.
 * @param y Coordonnée verticale de la cellule à vérifier.
 * @return int 0 si tous les voisins sont valides, -1 si un voisin est vide.
 *
 */
static int	check_neighbors_of(t_app *app, int x, int y)
{
	char	n;

	n = is_valid_neighbor(app, x - 1, y);
	if (!n)
		return (error_msg("Map not closed: walkable tile adjacent to void"));
	n = is_valid_neighbor(app, x + 1, y);
	if (!n)
		return (error_msg("Map not closed: walkable tile adjacent to void"));
	n = is_valid_neighbor(app, x, y - 1);
	if (!n)
		return (error_msg("Map not closed: walkable tile adjacent to void"));
	n = is_valid_neighbor(app, x, y + 1);
	if (!n)
		return (error_msg("Map not closed: walkable tile adjacent to void"));
	return (0);
}

/**
 * @brief Vérifie que la carte est correctement fermée (aucune ouverture).
 *
 * Pour chaque cellule walkable ('0' ou joueur), ses 4 voisins doivent
 * exister et ne pas être un espace ' '. Toute OOB ou espace est considérée
 * comme du vide (void) et invalide la carte.
 */
int	check_map_closed(t_app *app)
{
	int		y;
	int		x;
	char	c;

	y = 0;
	while (y < app->map.height)
	{
		x = 0;
		while (app->map.grid[y] && app->map.grid[y][x])
		{
			c = app->map.grid[y][x];
			if (c == '0' || c == 'N' || c == 'S'
				|| c == 'E' || c == 'W')
			{
				if (check_neighbors_of(app, x, y) != 0)
					return (-1);
			}
			x++;
		}
		y++;
	}
	return (0);
}

/**
 * @brief Valide la map complète : caractères, joueur et fermeture.
 *
 * Exécute la séquence complète de validation de la map en trois étapes :
 * 1. Vérification des caractères valides (check_valid_chars)
 * 2. Recherche et initialisation du joueur (find_player)
 * 3. Vérification de fermeture par des murs (check_map_closed)
 *
 * Cette fonction est le point d'entrée de la validation après le parsing.
 * Toute erreur provoque un error_msg() dans les sous-fonctions.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @return int Retourne 0 si toutes les validations passent.
 *
 */
int	validate_map(t_app *app)
{
	if (check_valid_chars(app) != 0)
		return (-1);
	if (find_player(app) != 0)
		return (-1);
	if (check_map_closed(app) != 0)
		return (-1);
	return (0);
}
