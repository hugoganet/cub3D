/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:56 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:57 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

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
 * @see cast_ray() qui utilise cette direction pour le DDA
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
 * @see fill_hit_info() dans dda.c qui calcule wall_face directement
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
 * @brief Rendu des murs avec couleurs unies (mode fallback sans textures).
 *
 * Effectue le raycasting pour chaque colonne d'écran et dessine les murs
 * avec des couleurs solides basées sur la face de mur (N/S/E/W). Utilisé
 * quand les textures ne sont pas chargées (app->tex.loaded == false).
 *
 * Algorithme :
 * 1. Calculer la direction du rayon pour la colonne x
 * 2. Lancer le rayon via DDA pour trouver le mur
 * 3. Calculer la hauteur et les bornes de dessin
 * 4. Dessiner la colonne avec la couleur de la face
 *
 * @param app Pointeur vers la structure principale.
 *
 * @see render_textured_walls() pour le rendu avec textures
 * @see get_wall_color() pour les couleurs par face
 */
static void	render_solid_colors(t_app *app)
{
	int			x;
	t_vec2		ray_dir;
	t_ray_hit	hit;
	int			params[3];

	printf("⚠️ Textures not loaded, using solid colors\n");
	x = 0;
	while (x < app->win_w)
	{
		calculate_ray_dir(app, x, &ray_dir);
		if (cast_ray(app, ray_dir, &hit))
		{
			calculate_wall_bounds((int)calculate_wall_height(hit.perp_dist,
					app->win_h), app->win_h, &params[1], &params[2]);
			params[0] = x;
			draw_wall_column(app, params, get_wall_color(hit.wall_face));
		}
		x++;
	}
}

/**
 * @brief Rendu des murs avec textures (mode normal).
 *
 * Effectue le raycasting pour chaque colonne d'écran et dessine les murs
 * avec les textures chargées (NO/SO/WE/EA). Utilisé quand app->tex.loaded
 * est true.
 *
 * Algorithme :
 * 1. Calculer la direction du rayon pour la colonne x
 * 2. Lancer le rayon via DDA pour trouver le mur
 * 3. Calculer la hauteur et les bornes de dessin
 * 4. Dessiner la colonne texturée avec échantillonnage vertical
 *
 * @param app Pointeur vers la structure principale avec textures chargées.
 *
 * @see render_solid_colors() pour le mode fallback
 * @see draw_textured_wall_column() pour le dessin avec textures
 */
static void	render_textured_walls(t_app *app)
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
					app->win_h), app->win_h, &params[1], &params[2]);
			params[0] = x;
			draw_textured_wall_column(app, params, &hit);
		}
		x++;
	}
}

/**
 * @brief Point d'entrée du rendu 3D (raycasting + projection).
 *
 * Sélectionne le mode de rendu selon l'état de chargement des textures :
 * - Si textures non chargées : rendu avec couleurs unies (fallback)
 * - Si textures chargées : rendu avec textures complètes
 *
 * Cette fonction est appelée à chaque frame dans la boucle de jeu.
 *
 * @param app Pointeur vers la structure principale.
 *
 * @see render_solid_colors() pour le mode sans textures
 * @see render_textured_walls() pour le mode avec textures
 * @see app_loop() dans loop.c qui appelle cette fonction
 */
void	render_3d_view(t_app *app)
{
	if (!app->tex.loaded)
		render_solid_colors(app);
	else
		render_textured_walls(app);
}
