/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:56 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/04 20:08:46 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

/* Indices pour le tableau de paramètres de colonne */
#define COL_X 0
#define COL_START 1
#define COL_END 2

/**
 * @brief Calcule la direction du rayon pour une colonne d'écran donnée.
 *
 * Transforme la coordonnée x de l'écran en coordonnée caméra dans [-1,1],
 * puis calcule le vecteur direction du rayon en combinant la direction
 * du joueur (dir) et le plan caméra (plane) :
 * ray_dir = dir + plane * camera_x
 *
 * Cette formule permet de balayer le champ de vision (FOV) de gauche à droite.
 *
 * @param app Pointeur vers la structure principale contenant player et win_w.
 * @param x Coordonnée x de la colonne d'écran (0 à win_w-1).
 * @param ray_dir Pointeur vers le vecteur résultat (direction du rayon).
 *
 */
void	calculate_ray_dir(t_app *app, int x, t_vec2 *ray_dir)
{
	double	camera_x;

	camera_x = 2 * x / (double)app->win_w - 1;
	ray_dir->x = app->player.dir.x + app->player.plane.x * camera_x;
	ray_dir->y = app->player.dir.y + app->player.plane.y * camera_x;
}

/**
 * @brief Détermine la face du mur touchée selon le côté et la direction.
 *
 * Analyse le côté de collision (0 = vertical, 1 = horizontal) et la
 * direction du pas (step_x/step_y) pour identifier la face de mur :
 * - Côté 0 : FACE_EAST (step_x < 0) ou FACE_WEST (step_x > 0)
 * - Côté 1 : FACE_NORTH (step_y > 0) ou FACE_SOUTH (step_y < 0)
 *
 * @param step_x Direction du pas sur l'axe X (-1 ou 1).
 * @param step_y Direction du pas sur l'axe Y (-1 ou 1).
 * @param side Côté de collision (0 = vertical, 1 = horizontal).
 * @return int Identifiant de la face de mur (FACE_NORTH/SOUTH/EAST/WEST).
 *
 */
int	get_wall_side(int step_x, int step_y, int side)
{
	if (side == 0)
	{
		if (step_x > 0)
			return (FACE_WEST);
		else
			return (FACE_EAST);
	}
	else
	{
		if (step_y > 0)
			return (FACE_NORTH);
		else
			return (FACE_SOUTH);
	}
}

/**
 * @brief Point d'entrée du rendu 3D (raycasting + projection).
 *
 * Effectue le raycasting pour chaque colonne d'écran et dessine les murs
 * avec les textures chargées (NO/SO/WE/EA).
 *
 * Algorithme :
 * 1. Calculer la direction du rayon pour la colonne x
 * 2. Lancer le rayon via DDA pour trouver le mur
 * 3. Calculer la hauteur et les bornes de dessin
 * 4. Dessiner la colonne texturée avec échantillonnage vertical
 *
 * Cette fonction est appelée à chaque frame dans la boucle de jeu.
 *
 * @param app Pointeur vers la structure principale avec textures chargées.
 *
 */
void	render_3d_view(t_app *app)
{
	int			x;
	t_vec2		ray_dir;
	t_ray_hit	hit;
	int			params[3];

	x = 0;
	while (x < app->win_w)
	{
		calculate_ray_dir(app, x, &ray_dir);
		if (cast_ray(app, ray_dir, &hit))
		{
			calculate_wall_bounds((int)calculate_wall_height(hit.perp_dist,
					app->win_h),
				app->win_h, &params[COL_START], &params[COL_END]);
			params[COL_X] = x;
			draw_textured_wall_column(app, params, &hit);
		}
		x++;
	}
}
