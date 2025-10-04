/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/02 13:06:50 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

/**
 * @brief Détermine la couleur d'une tuile selon son type
 *
 * Associe chaque caractère de la map à une couleur pour la minimap :
 * - '1' : blanc (mur)
 * - '0' : gris foncé (sol)
 * - 'N/S/E/W' : bleu (spawn joueur)
 * - ' ' : noir (vide)
 * - autre : magenta (erreur)
 *
 * @param tile Caractère de la map
 * @return int Couleur hexadécimale correspondante
 */
int	get_tile_color(char tile)
{
	if (tile == '1')
		return (0xFFFFFF);
	else if (tile == '0')
		return (0x404040);
	else if (tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W')
		return (0x0000FF);
	else if (tile == ' ')
		return (0x000000);
	else
		return (0xFF00FF);
}

/**
 * @brief Dessine la bordure blanche autour de la minimap
 *
 * Trace un rectangle blanc englobant toute la minimap avec une marge
 * de 2 pixels. La taille est calculée en fonction des dimensions de
 * la map et de l'échelle de la minimap.
 *
 * @param app Structure principale de l'application
 */
void	draw_minimap_border(t_app *app)
{
	int	border_params[4];

	border_params[0] = MINIMAP_OFFSET_X - 2;
	border_params[1] = MINIMAP_OFFSET_Y - 2;
	border_params[2] = app->map.width * MINIMAP_SCALE + 4;
	border_params[3] = app->map.height * MINIMAP_SCALE + 4;
	draw_rect(app, border_params, 0xFFFFFF);
}

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

/**
 * @brief Dessine un rayon du joueur vers un point de collision
 *
 * Trace une ligne verte du joueur vers le point où le rayon a touché
 * un mur. Les coordonnées sont converties en coordonnées minimap.
 *
 * @param app Structure principale de l'application
 * @param hit_point Point de collision du rayon dans les coordonnées monde
 */
void	draw_minimap_ray(t_app *app, t_vec2 hit_point)
{
	int	line_params[9];

	line_params[0] = MINIMAP_OFFSET_X
		+ (int)(app->player.pos.x * MINIMAP_SCALE);
	line_params[1] = MINIMAP_OFFSET_Y
		+ (int)(app->player.pos.y * MINIMAP_SCALE);
	line_params[2] = MINIMAP_OFFSET_X + (int)(hit_point.x * MINIMAP_SCALE);
	line_params[3] = MINIMAP_OFFSET_Y + (int)(hit_point.y * MINIMAP_SCALE);
	draw_line(app, line_params, COLOR_RAY);
}
