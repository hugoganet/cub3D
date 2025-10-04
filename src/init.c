/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:09 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/04 18:09:58 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

/**
 * @brief Crée l'image offscreen pour le double buffering.
 *
 * Alloue une image MLX de dimensions w×h et récupère son pointeur de données
 * pour permettre le dessin direct dans le buffer. Configure les métadonnées
 * de l'image (bpp, line_len, endian) pour l'accès aux pixels.
 *
 * @param app Pointeur vers la structure principale.
 * @param w Largeur de l'image en pixels.
 * @param h Hauteur de l'image en pixels.
 * @return int 0 si succès, -1 si erreur d'allocation MLX.
 *
 */
static int	create_frame(t_app *app, int w, int h)
{
	app->frame.ptr = mlx_new_image(app->mlx, w, h);
	if (!app->frame.ptr)
		return (-1);
	app->frame.addr = mlx_get_data_addr(app->frame.ptr, &app->frame.bpp,
			&app->frame.line_len,
			&app->frame.endian);
	app->frame.w = w;
	app->frame.h = h;
	return (0);
}

/**
 * @brief Initialise l'application complète (MLX, fenêtre, textures, frame).
 *
 * Séquence d'initialisation complète :
 * 1. Initialise MiniLibX
 * 2. Charge les 4 textures murales (NO/SO/WE/EA)
 * 3. Crée la fenêtre
 * 4. Crée l'image offscreen pour le rendu
 *
 * En cas d'erreur à n'importe quelle étape, affiche un message et retourne -1.
 *
 * @param app Pointeur vers la structure principale (doit être déjà parsée).
 * @param w Largeur de la fenêtre en pixels.
 * @param h Hauteur de la fenêtre en pixels.
 * @return int 0 si succès, -1 ou 1 en cas d'erreur.
 *
 */
int	app_init(t_app *app, int w, int h)
{
	app->win_w = w;
	app->win_h = h;
	app->mlx = mlx_init();
	if (!app->mlx)
		return (error_msg("mlx_init failed"));
	if (load_textures(app) < 0)
		return (error_msg("Failed to load textures"));
	app->win = mlx_new_window(app->mlx, w, h, "cub3D");
	if (!app->win)
		return (error_msg("mlx_new_window failed"));
	if (create_frame(app, w, h) != 0)
		return (error_msg("mlx_new_image failed"));
	return (0);
}
