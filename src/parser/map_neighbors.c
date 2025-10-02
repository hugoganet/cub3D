/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_neighbors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:48:04 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/02 13:06:49 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Détermine si une position est un mur ou hors limites.
 *
 * Vérifie si la position (x,y) correspond à :
 * - Une coordonnée hors de la grille (retourne 1)
 * - Un mur ('1') ou espace vide (' ') (retourne 1)
 * - Une position traversable ('0' ou joueur) (retourne 0)
 *
 * Utilisé pour valider la fermeture de la map et détecter les ouvertures.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @param x Coordonnée horizontale (colonne).
 * @param y Coordonnée verticale (ligne).
 * @return int 1 si position est mur/vide/OOB, 0 si traversable.
 *
 */
int is_wall_or_void(t_app *app, int x, int y)
{
	if (y < 0 || y >= app->map.height)
		return (1);
	if (x < 0 || !app->map.grid[y] || x >= (int)ft_strlen(app->map.grid[y]))
		return (1);
	if (app->map.grid[y][x] == '1' || app->map.grid[y][x] == ' ')
		return (1);
	return (0);
}

/**
 * @brief Vérifie si une cellule a un voisin ouvert (traversable).
 *
 * Teste les quatre voisins directs (gauche, droite, haut, bas) de la
 * position (x,y). Retourne 1 si au moins un voisin est traversable
 * (non-mur, non-vide), ce qui indique une ouverture potentielle.
 *
 * Utilisé pour vérifier que les positions de joueur ne sont pas
 * exposées au bord de la map.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @param x Coordonnée horizontale de la cellule à tester.
 * @param y Coordonnée verticale de la cellule à tester.
 * @return int 1 si au moins un voisin est ouvert, 0 sinon.
 *
 */
int has_open_neighbor(t_app *app, int x, int y)
{
	if (!is_wall_or_void(app, x - 1, y) || !is_wall_or_void(app, x + 1, y) || !is_wall_or_void(app, x, y - 1) || !is_wall_or_void(app, x, y + 1))
		return (1);
	return (0);
}

/**
 * @brief Détermine si une position est sur le bord de la map.
 *
 * Vérifie si la cellule (x,y) se trouve sur un des bords de la grille :
 * - Première ou dernière ligne (y == 0 ou y == height-1)
 * - Première ou dernière colonne de la ligne (x == 0 ou x == longueur-1)
 *
 * Utilisé pour détecter les positions de joueur vulnérables qui
 * pourraient permettre de sortir de la map.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @param x Coordonnée horizontale.
 * @param y Coordonnée verticale.
 * @return int 1 si la position est sur un bord, 0 sinon.
 *
 */
int is_at_map_edge(t_app *app, int x, int y)
{
	if (y == 0 || y == app->map.height - 1)
		return (1);
	if (x == 0)
		return (1);
	if (x == (int)ft_strlen(app->map.grid[y]) - 1)
		return (1);
	return (0);
}
