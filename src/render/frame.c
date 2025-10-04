/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:15:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/04 19:15:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Remplit entièrement une image avec une couleur unie
 *
 * Parcourt tous les pixels de l'image et applique la couleur spécifiée.
 * Utilisée pour nettoyer complètement le buffer de frame avant rendu.
 *
 * @param img Pointeur vers la structure d'image à remplir
 * @param color Couleur à appliquer (format hexadécimal)
 */
static void	clear_frame(t_img *img, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < img->h)
	{
		x = 0;
		while (x < img->w)
		{
			img_put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

/**
 * @brief Prépare le buffer de frame avec plafond et sol
 *
 * Nettoie le buffer de frame et dessine l'arrière-plan basique :
 * - Moitié supérieure : couleur du plafond
 * - Moitié inférieure : couleur du sol
 * Cette fonction est une alternative simplifiée à render_background.
 *
 * @param app Structure principale de l'application
 */
void	render_frame(t_app *app)
{
	int	floor_color;
	int	y;
	int	x;

	clear_frame(&app->frame, rgb_to_int(app->ceil));
	floor_color = rgb_to_int(app->floor);
	y = app->frame.h / 2;
	while (y < app->frame.h)
	{
		x = 0;
		while (x < app->frame.w)
		{
			img_put_pixel(&app->frame, x, y, floor_color);
			x++;
		}
		y++;
	}
}
