/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:30:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/01 13:25:11 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

/**
 * @brief Calcule la hauteur projetée d'un mur à l'écran.
 *
 * Applique la projection perspective pour calculer la hauteur du mur en pixels
 * selon sa distance au joueur. La formule est : hauteur = screen_h / distance.
 * Une distance minimale (0.001) évite la division par zéro 
 * et les murs infiniment grands.
 *
 * @param perp_dist Distance perpendiculaire au mur (corrigée fish-eye).
 * @param screen_h Hauteur de l'écran en pixels.
 * @return double Hauteur du mur en pixels (peut dépasser screen_h).
 *
 * @see calculate_wall_bounds() qui utilise cette hauteur pour le dessin
 */
double	calculate_wall_height(double perp_dist, int screen_h)
{
	if (perp_dist < 0.001)
		perp_dist = 0.001;
	return (screen_h / perp_dist);
}

/**
 * @brief Calcule la coordonnée X de texture depuis wall_x.
 *
 * Convertit la coordonnée d'impact sur le mur (wall_x ∈ [0,1]) en coordonnée
 * X dans la texture. Multiplie par la largeur de texture et clamp aux limites
 * pour éviter les accès hors bornes.
 *
 * @param wall_x Coordonnée d'impact normalisée sur le mur (0.0 à 1.0).
 * @param texture Pointeur vers la structure d'image texture.
 * @return int Coordonnée X dans la texture (0 à texture->w-1).
 *
 * @see fill_hit_info() dans dda.c qui calcule wall_x
 * @see init_texture_vars() qui utilise cette fonction
 */
int	get_texture_coord_x(double wall_x, t_img *texture)
{
	int	tex_x;

	tex_x = (int)(wall_x * (double)texture->w);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->w)
		tex_x = texture->w - 1;
	return (tex_x);
}

/**
 * @brief Retourne une couleur unie selon la face de mur (mode fallback).
 *
 * Fournit des couleurs différentes pour chaque face de mur afin de distinguer
 * visuellement l'orientation quand les textures ne sont pas chargées :
 * - FACE_NORTH : blanc (0xFFFFFF)
 * - FACE_SOUTH : gris clair (0xAAAAAA)
 * - FACE_EAST : gris moyen (0x888888)
 * - FACE_WEST : gris foncé (0x555555)
 *
 * @param wall_face Identifiant de la face de mur (FACE_NORTH/SOUTH/EAST/WEST).
 * @return int Couleur RGB au format 0xRRGGBB.
 *
 * @see render_solid_colors() qui utilise ces couleurs
 * @see draw_wall_column() pour l'application de la couleur
 */
int	get_wall_color(int wall_face)
{
	if (wall_face == FACE_NORTH)
		return (0xFFFFFF);
	else if (wall_face == FACE_SOUTH)
		return (0xAAAAAA);
	else if (wall_face == FACE_EAST)
		return (0x888888);
	else
		return (0x555555);
}
