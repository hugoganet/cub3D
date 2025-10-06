/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:30 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/06 16:01:37 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

/**
 * @brief Initialise les variables DDA pour l'axe X et calcule deltaDistX.
 *
 * Prépare les variables nécessaires à l'algorithme DDA (Digital Differential
 * Analyzer) pour le raycasting :
 * - map_x/map_y : position initiale dans la grille (cellule du joueur)
 * - delta_dist : distance parcourue par le rayon pour traverser une cellule
 * - step_x : direction du pas (-1 ou 1)
 * - side_dist_x : distance initiale jusqu'au premier bord de cellule
 *
 * @param app Pointeur vers la structure principale (position joueur).
 * @param ray_dir Vecteur direction du rayon.
 * @param vars Pointeur vers la structure des variables DDA à initialiser.
 *
 */
static void	init_dda_vars(t_app *app, t_vec2 ray_dir, t_ray_vars *vars)
{
	vars->map_x = (int)app->player.pos.x;
	vars->map_y = (int)app->player.pos.y;
	if (ray_dir.x == 0)
		vars->delta_dist_x = 1e30;
	else
		vars->delta_dist_x = fabs(1 / ray_dir.x);
	if (ray_dir.y == 0)
		vars->delta_dist_y = 1e30;
	else
		vars->delta_dist_y = fabs(1 / ray_dir.y);
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
 * @brief Initialise les variables DDA pour l'axe Y.
 *
 * Complète l'initialisation des variables DDA en calculant step_y et
 * side_dist_y selon la direction du rayon sur l'axe Y. Travaille en
 * tandem avec init_dda_vars() pour préparer l'algorithme DDA complet.
 *
 * @param app Pointeur vers la structure principale (position joueur).
 * @param ray_dir Vecteur direction du rayon.
 * @param vars Pointeur vers la structure des variables DDA à compléter.
 *
 */
static void	init_dda_vars_y(t_app *app, t_vec2 ray_dir, t_ray_vars *vars)
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
 * @brief Vérifie si le rayon a touché un mur à la position actuelle.
 *
 * Teste si la cellule de grille (map_x, map_y) contient un mur ('1') ou
 * est hors limites. Utilisé dans la boucle DDA pour détecter la fin du
 * raycasting.
 *
 * Conditions de collision :
 * - Position hors des limites de la grille
 * - Caractère '1' dans la grille (mur)
 *
 * @param app Pointeur vers la structure principale contenant la map.
 * @param vars Pointeur vers les variables DDA avec position courante.
 * @return int 1 si mur touché ou OOB, 0 sinon.
 *
 */
static int	check_wall_hit(t_app *app, t_ray_vars *vars)
{
	if (vars->map_x < 0 || vars->map_y < 0 || vars->map_x >= app->map.width
		|| vars->map_y >= app->map.height)
		return (1);
	if (app->map.grid[vars->map_y]
		&& vars->map_x < (int)ft_strlen(app->map.grid[vars->map_y])
		&& app->map.grid[vars->map_y][vars->map_x] == '1')
		return (1);
	return (0);
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
static void	fill_hit_info(t_app *app, t_vec2 ray_dir, t_ray_vars *vars,
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

/**
 * @brief Lance un rayon et détecte la collision avec un mur (algorithme DDA).
 *
 * Implémente l'algorithme Digital Differential Analyzer (DDA) pour tracer
 * un rayon dans la grille 2D jusqu'à toucher un mur. Avance case par case
 * en choisissant l'axe dont la distance au prochain bord est minimale.
 *
 * Algorithme :
 * 1. Initialiser les variables DDA (delta_dist, step, side_dist)
 * 2. Boucler jusqu'à collision :
 *    - Avancer sur l'axe X ou Y selon side_dist minimal
 *    - Vérifier si mur touché
 * 3. Calculer les informations de collision (distance, face, coordonnées)
 *
 * @param app Pointeur vers la structure principale (joueur, map).
 * @param ray_dir Vecteur direction du rayon normalisé.
 * @param hit Pointeur vers la structure t_ray_hit à remplir.
 * @return int Retourne toujours 1 (collision trouvée).
 *
 */
int	cast_ray(t_app *app, t_vec2 ray_dir, t_ray_hit *hit)
{
	t_ray_vars	vars;
	int			wall_hit;
	// ? refacto
	init_dda_vars(app, ray_dir, &vars);
	init_dda_vars_y(app, ray_dir, &vars);
	wall_hit = 0;
	while (wall_hit == 0)
	{
		if (vars.side_dist_x < vars.side_dist_y)
		{
			vars.side_dist_x += vars.delta_dist_x;
			vars.map_x += vars.step_x;
			vars.side = 0;
		}
		else
		{
			vars.side_dist_y += vars.delta_dist_y;
			vars.map_y += vars.step_y;
			vars.side = 1;
		}
		wall_hit = check_wall_hit(app, &vars);
	}
	fill_hit_info(app, ray_dir, &vars, hit);
	return (1);
}
