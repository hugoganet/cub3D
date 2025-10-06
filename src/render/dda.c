/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:30 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/06 17:02:36 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
int	check_wall_hit(t_app *app, t_ray_vars *vars)
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

	init_dda_vars(app, ray_dir, &vars);
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
