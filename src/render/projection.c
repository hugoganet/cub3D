/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:39 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/03 18:04:31 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

/**
 * @brief Calcule les bornes verticales de dessin d'une colonne de mur.
 *
 * Détermine les coordonnées Y de début (draw_start) et fin (draw_end) pour
 * dessiner une colonne de mur à l'écran. Centre la bande verticalement et
 * clamp les valeurs aux limites de l'écran pour éviter les dépassements.
 *
 * Calcul :
 *
 * - centre = screen_h/2
 * - début = centre - height/2
 * - fin = centre + height/2
 *
 * @param height Hauteur projetée du mur en pixels.
 * @param screen_h Hauteur de l'écran en pixels.
 * @param draw_start Pointeur vers la coordonnée Y de début (résultat).
 * @param draw_end Pointeur vers la coordonnée Y de fin (résultat).
 *
 */
void	calculate_wall_bounds(int height, int screen_h, int *draw_start,
		int *draw_end)
{
	*draw_start = -height / 2 + screen_h / 2;
	if (*draw_start < 0)
		*draw_start = 0;
	*draw_end = height / 2 + screen_h / 2;
	if (*draw_end >= screen_h)
		*draw_end = screen_h - 1;
}

/**
 * @brief Dessine une colonne verticale de mur avec une couleur unie.
 *
 * Remplit tous les pixels d'une colonne d'écran avec la couleur spécifiée.
 * Utilisé dans le mode fallback (sans textures) pour le rendu des murs.
 *
 * @param app Pointeur vers la structure principale (contient frame).
 * @param params Tableau [x, draw_start, draw_end] définissant la colonne.
 * @param color Couleur RGB à appliquer (format 0xRRGGBB).
 *
 */
void	draw_wall_column(t_app *app, int *params, int color)
{
	int	y;

	y = params[1];
	while (y <= params[2])
	{
		img_put_pixel(&app->frame, params[0], y, color);
		y++;
	}
}

/**
 * @brief Sélectionne la texture appropriée selon la face de mur touchée.
 *
 * Retourne un pointeur vers la texture correspondant à la face de mur :
 * - FACE_NORTH → texture NO (nord)
 * - FACE_SOUTH → texture SO (sud)
 * - FACE_EAST → texture EA (est)
 * - FACE_WEST → texture WE (ouest)
 *
 * @param app Pointeur vers la structure principale contenant les textures.
 * @param wall_face Identifiant de la face de mur (FACE_NORTH/SOUTH/EAST/WEST).
 * @return t_img* Pointeur vers la structure d'image texture.
 *
 */
static t_img	*get_texture_from_face(t_app *app, int wall_face)
{
	if (wall_face == FACE_NORTH)
		return (&app->tex.north);
	else if (wall_face == FACE_SOUTH)
		return (&app->tex.south);
	else if (wall_face == FACE_EAST)
		return (&app->tex.east);
	else
		return (&app->tex.west);
}

static void	init_texture_vars(t_app *app, t_ray_hit *hit,
		int draw_start, double *vars)
{
	t_img	*texture;
	int		line_height;

	texture = get_texture_from_face(app, hit->wall_face);
	vars[0] = get_texture_coord_x(hit->wall_x, texture, hit->wall_face);
	line_height = (int)(app->win_h / hit->perp_dist);
	vars[1] = (double)texture->h / (double)line_height;
	vars[2] = (draw_start - app->win_h / 2 + line_height / 2) * vars[1];
}

/**
 * @brief Dessine une colonne verticale de mur avec échantillonnage de texture.
 *
 * Effectue le texture mapping pour une colonne d'écran en échantillonnant
 * la texture appropriée verticalement. Pour chaque pixel :
 * 1. Calculer la coordonnée Y dans la texture (tex_y)
 * 2. Clamper tex_y aux limites de la texture
 * 3. Récupérer la couleur du pixel texture (tex_x, tex_y)
 * 4. Dessiner le pixel à l'écran
 * 5. Incrémenter tex_y selon le step
 *
 * @param app Pointeur vers la structure principale (frame, textures).
 * @param params Tableau [x, draw_start, draw_end] définissant la colonne.
 * @param hit Pointeur vers les données de collision (wall_face, wall_x).
 *
 */
void	draw_textured_wall_column(t_app *app, int *params, t_ray_hit *hit)
{
	t_img		*texture;
	double		vars[3];
	int			y;
	int			tex_y;

	texture = get_texture_from_face(app, hit->wall_face);
	init_texture_vars(app, hit, params[1], vars);
	y = params[1];
	while (y <= params[2])
	{
		tex_y = (int)vars[2];
		if (tex_y >= texture->h)
			tex_y = texture->h - 1;
		if (tex_y < 0)
			tex_y = 0;
		img_put_pixel(&app->frame, params[0], y,
			get_texture_pixel(texture, vars[0], tex_y));
		vars[2] += vars[1];
		y++;
	}
}
