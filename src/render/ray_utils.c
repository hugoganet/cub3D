/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:42 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:43 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Lance un rayon pour la minimap et calcule le point d'impact.
 *
 * Version simplifiée de cast_ray() dédiée à la minimap. Lance un rayon
 * via DDA et convertit la distance de collision en coordonnées 2D du
 * point d'impact sur le mur. Utilisé pour dessiner les rayons visibles
 * sur la minimap.
 *
 * @param app Pointeur vers la structure principale.
 * @param ray_dir Vecteur direction du rayon.
 * @param hit_point Pointeur vers le vecteur résultat (coordonnées d'impact).
 * @return int 1 si collision trouvée (toujours vrai avec cast_ray actuel).
 *
 * @see cast_ray() pour l'algorithme DDA complet
 * @see render_minimap_rays() qui appelle cette fonction
 * @see draw_minimap_ray() qui dessine la ligne résultante
 */
int	cast_minimap_ray(t_app *app, t_vec2 ray_dir, t_vec2 *hit_point)
{
	t_ray_hit	hit;

	if (!cast_ray(app, ray_dir, &hit))
		return (0);
	hit_point->x = app->player.pos.x + hit.perp_dist * ray_dir.x;
	hit_point->y = app->player.pos.y + hit.perp_dist * ray_dir.y;
	return (1);
}
