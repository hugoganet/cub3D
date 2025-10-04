/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:35 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/04 20:09:51 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* Indices pour le tableau de paramètres de ligne Bresenham */
#define X 0
#define Y 1
#define END_X 2
#define END_Y 3
#define DELT_X 4
#define DELT_Y 5
#define STEP_X 6
#define STEP_Y 7
#define ERR 8

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
 * @brief Dessine un rectangle rempli à l'écran
 *
 * Parcourt tous les pixels du rectangle et applique la couleur spécifiée.
 * Utilise deux boucles imbriquées pour remplir ligne par ligne.
 *
 * @param app Structure principale de l'application
 * @param rect Tableau [start_x, start_y, width, height] du rectangle
 * @param color Couleur de remplissage (format hexadécimal)
 */
void	draw_rect(t_app *app, int *rect, int color)
{
	int	x;
	int	y;
	int	start_x;
	int	start_y;

	start_x = rect[0];
	start_y = rect[1];
	y = 0;
	while (y < rect[3])
	{
		x = 0;
		while (x < rect[2])
		{
			img_put_pixel(&app->frame, start_x + x, start_y + y, color);
			x++;
		}
		y++;
	}
}

/**
 * @brief Initialise les paramètres de Bresenham pour le tracé de ligne
 *
 * Calcule les deltas (dx, dELT_Y), les directions de pas (step_x, step_y)
 * et l'erreur initiale pour l'algorithme de Bresenham.
 *
 * @param line Tableau des paramètres de ligne
 */
static void	init_bresenham(int *line)
{
	int	delta_x;
	int	delta_y;

	delta_x = abs(line[END_X] - line[X]);
	delta_y = abs(line[END_Y] - line[Y]);
	line[DELT_X] = delta_x;
	line[DELT_Y] = delta_y;
	if (line[X] < line[END_X])
		line[STEP_X] = 1;
	else
		line[STEP_X] = -1;
	if (line[Y] < line[END_Y])
		line[STEP_Y] = 1;
	else
		line[STEP_Y] = -1;
	line[ERR] = delta_x - delta_y;
}

/**
 * @brief Dessine une ligne entre deux points avec l'algorithme de Bresenham
 *
 * Trace une ligne droite entre deux points sans anti-aliasing.
 * L'algorithme de Bresenham évite les calculs en virgule flottante.
 *
 * Utilise un système d'accumulation d'erreur pour déterminer quand
 * avancer dans la direction X ou Y, produisant une ligne visuellement
 * droite même sur une grille de pixels discrète.
 *
 * @param app Structure principale de l'application
 * @param line Tableau des paramètres [x, y, end_x, end_y, ...]
 * @param color Couleur de la ligne (format hexadécimal)
 */
void	draw_line(t_app *app, int *line, int color)
{
	int	error_doubled;

	init_bresenham(line);
	while (1)
	{
		img_put_pixel(&app->frame, line[X], line[Y], color);
		if (line[X] == line[END_X] && line[Y] == line[END_Y])
			break ;
		error_doubled = 2 * line[ERR];
		if (error_doubled > -line[DELT_Y])
		{
			line[ERR] -= line[DELT_Y];
			line[X] += line[STEP_X];
		}
		if (error_doubled < line[DELT_X])
		{
			line[ERR] += line[DELT_X];
			line[Y] += line[STEP_Y];
		}
	}
}
