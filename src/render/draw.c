/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:35 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:32:36 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>
#include <stdlib.h>

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

/**
 * @brief Place un pixel coloré dans une image à une position donnée
 *
 * Écrit directement dans le buffer de l'image MLX. Inclut une vérification
 * des limites pour éviter les accès hors-bornes (hardcodé 1024x768).
 *
 * @param img Pointeur vers la structure d'image cible
 * @param x Coordonnée X du pixel
 * @param y Coordonnée Y du pixel
 * @param color Couleur du pixel (format hexadécimal)
 */
void	img_put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= 1024 || y >= 768)
		return ;
	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

/**
 * @brief Remplit tout l'écran avec une couleur unie
 *
 * Parcourt tous les pixels du buffer de frame et applique la couleur
 * spécifiée. Utilise les dimensions de la fenêtre (win_w, win_h).
 * Fonction utilitaire pour nettoyer complètement l'écran.
 *
 * @param app Structure principale de l'application
 * @param color Couleur de remplissage (format hexadécimal)
 */
void	fill_background(t_app *app, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < app->win_h)
	{
		x = 0;
		while (x < app->win_w)
		{
			img_put_pixel(&app->frame, x, y, color);
			x++;
		}
		y++;
	}
}
