/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_player.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:47:11 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 16:44:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

/**
 * @brief Traite une cellule pour détecter et initialiser le joueur.
 *
 * Si la cellule (i,j) contient un caractère de joueur (N/S/E/W) :
 * - incrémente le compteur de joueurs (*player_count),
 * - positionne app->player.pos au centre de la cellule,
 * - définit l'orientation du joueur via set_player_orientation().
 * Si la cellule ne contient pas un caractère de joueur, ne fait rien.
 *
 * @param app Ptr vers la structure principale contenant la map et le player.
 * @param i   Indice de ligne (y) dans la grille.
 * @param j   Indice de colonne (x) dans la grille.
 * @param player_count Pointeur vers le compteur de joueurs trouvé jusqu'ici.
 */
void	process_cell_for_player(t_app *app, int i, int j, int *player_count)
{
	char	c;

	c = app->map.grid[i][j];
	if (!is_player_char(c))
		return ;
	(*player_count)++;
	app->player.pos.x = (double)j + 0.5;
	app->player.pos.y = (double)i + 0.5;
	set_player_orientation(app, c);
}

/**
 * @brief Détermine si un caractère représente un joueur.
 *
 * Vérifie si le caractère correspond à l'une des orientations de départ
 * possibles du joueur : Nord (N), Sud (S), Est (E) ou Ouest (W).
 *
 * @param c Caractère à tester.
 * @return int 1 si c est un caractère joueur, 0 sinon.
 *
 */
int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/**
 * @brief Configure l'orientation du joueur pour Nord ou Sud.
 *
 * Définit les vecteurs de direction (dir) et de plan caméra (plane) pour
 * une orientation Nord (N) ou Sud (S) :
 * - Nord : dir=(0,-1), plane=(0.66,0) - regarde vers le haut
 * - Sud : dir=(0,1), plane=(-0.66,0) - regarde vers le bas
 *
 * La magnitude du plane (0.66) détermine le FOV (~60°).
 *
 * @param app Pointeur vers la structure principale.
 * @param c Caractère d'orientation ('N' ou 'S').
 *
 */
void	orientation_north_or_south(t_app *app, char c)
{
	if (c == 'N')
	{
		app->player.dir.x = 0;
		app->player.dir.y = -1;
		app->player.plane.x = 0.66;
		app->player.plane.y = 0;
	}
	else if (c == 'S')
	{
		app->player.dir.x = 0;
		app->player.dir.y = 1;
		app->player.plane.x = -0.66;
		app->player.plane.y = 0;
	}
}

/**
 * @brief Configure l'orientation du joueur pour Est ou Ouest.
 *
 * Définit les vecteurs de direction (dir) et de plan caméra (plane) pour
 * une orientation Est (E) ou Ouest (W) :
 * - Est : dir=(1,0), plane=(0,0.66) - regarde vers la droite
 * - Ouest : dir=(-1,0), plane=(0,-0.66) - regarde vers la gauche
 *
 * Le plane est perpendiculaire à dir avec magnitude 0.66 pour FOV ~60°.
 *
 * @param app Pointeur vers la structure principale.
 * @param c Caractère d'orientation ('E' ou 'W').
 *
 */
void	orientation_east_or_west(t_app *app, char c)
{
	if (c == 'E')
	{
		app->player.dir.x = 1;
		app->player.dir.y = 0;
		app->player.plane.x = 0;
		app->player.plane.y = 0.66;
	}
	else if (c == 'W')
	{
		app->player.dir.x = -1;
		app->player.dir.y = 0;
		app->player.plane.x = 0;
		app->player.plane.y = -0.66;
	}
}

/**
 * @brief Définit l'orientation initiale du joueur selon le caractère de spawn.
 *
 * Dispatche vers orientation_north_or_south() ou orientation_east_or_west()
 * selon le caractère de départ (N/S/E/W) trouvé dans la map. Configure les
 * vecteurs dir et plane pour le raycasting.
 *
 * @param app Pointeur vers la structure principale.
 * @param c Caractère d'orientation du joueur ('N', 'S', 'E', 'W').
 *
 */
void	set_player_orientation(t_app *app, char c)
{
	if (c == 'N' || c == 'S')
		orientation_north_or_south(app, c);
	if (c == 'E' || c == 'W')
		orientation_east_or_west(app, c);
}
