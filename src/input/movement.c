/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:20:04 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/02 13:12:45 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

/**
 * @brief Vérifie si une position est traversable (collision detection).
 *
 * Teste si les coordonnées (x, y) correspondent à une cellule valide et
 * non-mur dans la grille de map. Utilisé pour empêcher le joueur de
 * traverser les murs ou de sortir de la map.
 *
 * Conditions pour position valide :
 * - Coordonnées dans les limites de la grille
 * - Cellule existe (ligne non-NULL)
 * - Caractère différent de '1' (mur)
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @param x Coordonnée X à tester (en unités de grille, pas pixels).
 * @param y Coordonnée Y à tester (en unités de grille, pas pixels).
 * @return bool true si position traversable, false sinon.
 *
 */
bool	is_valid_position(t_app *app, double x, double y)
{
	int		map_x;
	int		map_y;
	char	tile;

	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_y < 0 || map_y >= app->map.height)
		return (false);
	if (!app->map.grid[map_y])
		return (false);
	if (map_x >= (int)ft_strlen(app->map.grid[map_y]))
		return (false);
	tile = app->map.grid[map_y][map_x];
	if (tile == '1')
		return (false);
	return (true);
}

/**
 * @brief Applique une rotation au joueur (direction + plan caméra).
 *
 * Effectue une rotation 2D des vecteurs dir et plane du joueur via une
 * matrice de rotation. L'angle de rotation est : rot_speed × direction.
 *
 * Formule de rotation 2D :
 * - new_x = x × cos(θ) - y × sin(θ)
 * - new_y = x × sin(θ) + y × cos(θ)
 *
 * @param app Pointeur vers la structure principale.
 * @param direction Sens de rotation (-1 = gauche, 1 = droite).
 *
 */
void	rotate_player(t_app *app, int direction)
{
	double	angle;
	double	old_dir_x;
	double	old_plane_x;

	angle = app->rot_speed * direction;
	old_dir_x = app->player.dir.x;
	old_plane_x = app->player.plane.x;
	app->player.dir.x = app->player.dir.x * cos(angle)
		- app->player.dir.y * sin(angle);
	app->player.dir.y = old_dir_x * sin(angle)
		+ app->player.dir.y * cos(angle);
	app->player.plane.x = app->player.plane.x * cos(angle)
		- app->player.plane.y * sin(angle);
	app->player.plane.y = old_plane_x * sin(angle)
		+ app->player.plane.y * cos(angle);
}

/**
 * @brief Déplace le joueur avec détection de collision.
 *
 * Calcule la nouvelle position du joueur en ajoutant le vecteur de déplacement
 * (move_x, move_y) multiplié par move_speed. Teste la collision sur chaque axe
 * indépendamment pour permettre le "sliding" le long des murs.
 *
 * Algorithme :
 * 1. Calculer new_x = pos.x + move_x × speed
 * 2. Si (new_x, pos.y) valide → appliquer déplacement X
 * 3. Calculer new_y = pos.y + move_y × speed
 * 4. Si (pos.x, new_y) valide → appliquer déplacement Y
 *
 * @param app Pointeur vers la structure principale.
 * @param move_x Composante X du vecteur de déplacement (normalisée).
 * @param move_y Composante Y du vecteur de déplacement (normalisée).
 *
 */
void	move_player(t_app *app, double move_x, double move_y)
{
	double	new_x;
	double	new_y;

	new_x = app->player.pos.x + move_x * app->move_speed;
	new_y = app->player.pos.y + move_y * app->move_speed;
	if (is_valid_position(app, new_x, app->player.pos.y))
		app->player.pos.x = new_x;
	if (is_valid_position(app, app->player.pos.x, new_y))
		app->player.pos.y = new_y;
}

/**
 * @brief Calcule le vecteur de déplacement selon les touches pressées.
 *
 * Accumule les composantes X et Y du mouvement souhaité en fonction
 * des touches actives (WASD) :
 * - W : avance (dir)
 * - S : recule (-dir)
 * - A : strafe gauche (perpendiculaire à dir, rotation -90°)
 * - D : strafe droite (perpendiculaire à dir, rotation +90°)
 *
 * Le vecteur résultant peut être diagonal si plusieurs touches sont pressées.
 *
 * @param app Pointeur vers la structure principale (keys, player.dir).
 * @param move_x Pointeur vers la composante X accumulée (sortie).
 * @param move_y Pointeur vers la composante Y accumulée (sortie).
 *
 */
static void	apply_movement(t_app *app, double *move_x, double *move_y)
{
	if (app->keys.w)
	{
		*move_x += app->player.dir.x;
		*move_y += app->player.dir.y;
	}
	if (app->keys.s)
	{
		*move_x -= app->player.dir.x;
		*move_y -= app->player.dir.y;
	}
	if (app->keys.a)
	{
		*move_x += app->player.dir.y;
		*move_y -= app->player.dir.x;
	}
	if (app->keys.d)
	{
		*move_x -= app->player.dir.y;
		*move_y += app->player.dir.x;
	}
}

/**
 * @brief Point d'entrée de mise à jour du joueur (appelé chaque frame).
 *
 * Orchestre la mise à jour complète du joueur selon les touches actives :
 * 1. Calcule le vecteur de déplacement via apply_movement()
 * 2. Applique le déplacement si non-nul (avec collision)
 * 3. Applique la rotation gauche si LEFT pressée
 * 4. Applique la rotation droite si RIGHT pressée
 *
 * Cette fonction est appelée dans app_loop() avant le rendu.
 *
 * @param app Pointeur vers la structure principale (keys, player).
 *
 */
void	update_player_movement(t_app *app)
{
	double	move_x;
	double	move_y;

	move_x = 0;
	move_y = 0;
	apply_movement(app, &move_x, &move_y);
	if (move_x != 0 || move_y != 0)
		move_player(app, move_x, move_y);
	if (app->keys.left)
		rotate_player(app, -1);
	if (app->keys.right)
		rotate_player(app, 1);
}
