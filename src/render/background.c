/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:27 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:28 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Convertit une structure de couleur RGB en entier hexadécimal
 *
 * @param color Structure contenant les composantes RGB de la couleur
 * @return int Valeur hexadécimale de la couleur (format 0xRRGGBB)
 * @see t_color
 */
int	color_to_int(t_color color)
{
	return ((color.r << 16) | (color.g << 8) | color.b);
}

/**
 * @brief Dessine le plafond avec une couleur unie
 *
 * Remplit la moitié supérieure de l'écran avec la couleur du plafond
 * définie dans le fichier .cub. Utilise draw_rect pour dessiner des
 * bandes horizontales ligne par ligne.
 *
 * @param app Structure principale de l'application contenant les couleurs
 * @see draw_rect
 * @see color_to_int
 */
void	draw_ceiling(t_app *app)
{
	int	ceiling_color;
	int	y;
	int	half_height;
	int	params[4];

	ceiling_color = color_to_int(app->ceil);
	y = 0;
	half_height = DEFAULT_WIN_HEIGHT / 2;
	while (y < half_height)
	{
		params[0] = 0;
		params[1] = y;
		params[2] = DEFAULT_WIN_WIDTH;
		params[3] = 1;
		draw_rect(app, params, ceiling_color);
		y++;
	}
}

/**
 * @brief Dessine le sol avec une couleur unie
 *
 * Remplit la moitié inférieure de l'écran avec la couleur du sol
 * définie dans le fichier .cub. Utilise draw_rect pour dessiner des
 * bandes horizontales ligne par ligne.
 *
 * @param app Structure principale de l'application contenant les couleurs
 * @see draw_rect
 * @see color_to_int
 */
void	draw_floor(t_app *app)
{
	int	floor_color;
	int	y;
	int	params[4];

	floor_color = color_to_int(app->floor);
	y = DEFAULT_WIN_HEIGHT / 2;
	while (y < DEFAULT_WIN_HEIGHT)
	{
		params[0] = 0;
		params[1] = y;
		params[2] = DEFAULT_WIN_WIDTH;
		params[3] = 1;
		draw_rect(app, params, floor_color);
		y++;
	}
}

/**
 * @brief Rendu complet de l'arrière-plan (plafond et sol)
 *
 * Fonction principale pour dessiner l'arrière-plan de la scène 3D.
 * Appelle successivement draw_ceiling et draw_floor pour créer
 * le fond avant le rendu des murs.
 *
 * @param app Structure principale de l'application
 * @see draw_ceiling
 * @see draw_floor
 */
void	render_background(t_app *app)
{
	draw_ceiling(app);
	draw_floor(app);
}
