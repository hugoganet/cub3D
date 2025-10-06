/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:47:00 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/06 17:02:29 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

/**
 * @brief Initialise les delta_dist pour les axes X et Y.
 *
 * Calcule la distance que le rayon doit parcourir pour traverser une cellule
 * complète sur chaque axe. Utilise 1e30 si la direction est nulle pour
 * éviter la division par zéro.
 *
 * @param ray_dir Vecteur direction du rayon.
 * @param vars Pointeur vers la structure des variables DDA à initialiser.
 *
 */
void	init_delta_dist(t_vec2 ray_dir, t_ray_vars *vars)
{
	if (ray_dir.x == 0)
		vars->delta_dist_x = 1e30;
	else
		vars->delta_dist_x = fabs(1 / ray_dir.x);
	if (ray_dir.y == 0)
		vars->delta_dist_y = 1e30;
	else
		vars->delta_dist_y = fabs(1 / ray_dir.y);
}

/**
 * @brief Initialise les variables de pas et distances pour l'axe X.
 *
 * Calcule step_x (-1 ou 1) et la distance initiale jusqu'au prochain
 * bord de cellule selon la direction du rayon.
 *
 * @param app Pointeur vers la structure principale (position joueur).
 * @param ray_dir Vecteur direction du rayon.
 * @param vars Pointeur vers la structure des variables DDA.
 *
 */
void	init_step_x(t_app *app, t_vec2 ray_dir, t_ray_vars *vars)
{
	if (ray_dir.x < 0)
	{
		vars->step_x = -1;
		vars->side_dist_x = (app->player.pos.x - vars->map_x)
			* vars->delta_dist_x;
	}
	else
	{
		vars->step_x = 1;
		vars->side_dist_x = (vars->map_x + 1.0 - app->player.pos.x)
			* vars->delta_dist_x;
	}
}

/**
 * @brief Initialise les variables de pas et distances pour l'axe Y.
 *
 * Calcule step_y (-1 ou 1) et la distance initiale jusqu'au prochain
 * bord de cellule selon la direction du rayon.
 *
 * @param app Pointeur vers la structure principale (position joueur).
 * @param ray_dir Vecteur direction du rayon.
 * @param vars Pointeur vers la structure des variables DDA.
 *
 */
void	init_step_y(t_app *app, t_vec2 ray_dir, t_ray_vars *vars)
{
	if (ray_dir.y < 0)
	{
		vars->step_y = -1;
		vars->side_dist_y = (app->player.pos.y - vars->map_y)
			* vars->delta_dist_y;
	}
	else
	{
		vars->step_y = 1;
		vars->side_dist_y = (vars->map_y + 1.0 - app->player.pos.y)
			* vars->delta_dist_y;
	}
}

/**
 * @brief Initialise toutes les variables DDA pour le raycasting.
 *
 * Prépare l'ensemble des variables nécessaires à l'algorithme DDA :
 * - map_x/map_y : position initiale dans la grille (cellule du joueur)
 * - delta_dist : distance parcourue par le rayon pour traverser une cellule
 * - step : direction du pas (-1 ou 1) pour chaque axe
 * - side_dist : distance initiale jusqu'au premier bord de cellule
 *
 * @param app Pointeur vers la structure principale (position joueur).
 * @param ray_dir Vecteur direction du rayon.
 * @param vars Pointeur vers la structure des variables DDA à initialiser.
 *
 */
void	init_dda_vars(t_app *app, t_vec2 ray_dir, t_ray_vars *vars)
{
	vars->map_x = (int)app->player.pos.x;
	vars->map_y = (int)app->player.pos.y;
	init_delta_dist(ray_dir, vars);
	init_step_x(app, ray_dir, vars);
	init_step_y(app, ray_dir, vars);
}

/**
 * @brief Remplit la structure t_ray_hit avec les informations de collision.
 *
 * Calcule et stocke toutes les données de collision nécessaires au rendu :
 * - perp_dist : distance perpendiculaire (corrige fish-eye)
 * - wall_face : face de mur touchée (N/S/E/W)
 * - wall_x : coordonnée d'impact sur le mur (pour texture mapping)
 * - map_x/map_y : position de la cellule touchée
 *
 * La distance perpendiculaire est calculée selon le côté de collision
 * (0=vertical, 1=horizontal) pour éviter l'effet fish-eye.
 *
 * @param app Pointeur vers la structure principale (position joueur).
 * @param ray_dir Vecteur direction du rayon.
 * @param vars Pointeur vers les variables DDA finales.
 * @param hit Pointeur vers la structure t_ray_hit à remplir.
 *
 */
void	fill_hit_info(t_app *app, t_vec2 ray_dir, t_ray_vars *vars,
		t_ray_hit *hit)
{
	if (vars->side == 0)
		hit->perp_dist = (vars->map_x - app->player.pos.x
				+ (1 - vars->step_x) / 2) / ray_dir.x;
	else
		hit->perp_dist = (vars->map_y - app->player.pos.y
				+ (1 - vars->step_y) / 2) / ray_dir.y;
	hit->side = vars->side;
	hit->map_x = vars->map_x;
	hit->map_y = vars->map_y;
	if (vars->side == 0 && vars->step_x > 0)
		hit->wall_face = FACE_WEST;
	else if (vars->side == 0)
		hit->wall_face = FACE_EAST;
	else if (vars->step_y > 0)
		hit->wall_face = FACE_NORTH;
	else
		hit->wall_face = FACE_SOUTH;
	if (vars->side == 0)
		hit->wall_x = app->player.pos.y + hit->perp_dist * ray_dir.y;
	else
		hit->wall_x = app->player.pos.x + hit->perp_dist * ray_dir.x;
	hit->wall_x -= floor(hit->wall_x);
}
