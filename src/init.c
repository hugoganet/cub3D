/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:09 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/02 13:08:10 by hugoganet        ###   ########.fr       */
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
static int create_frame(t_app *app, int w, int h)
{
	app->frame.ptr = mlx_new_image(app->mlx, w, h);
	if (!app->frame.ptr)
		return (-1);
	app->frame.addr = mlx_get_data_addr(app->frame.ptr, &app->frame.bpp,
										&app->frame.line_len, &app->frame.endian);
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
int app_init(t_app *app, int w, int h)
{
	app->win_w = w;
	app->win_h = h;
	app->mlx = mlx_init();
	if (!app->mlx)
		return (ft_putendl_fd("Error\nmlx_init failed", 2), -1);
	if (load_textures(app) != 0)
		return (1);
	app->win = mlx_new_window(app->mlx, w, h, "cub3D");
	if (!app->win)
		return (ft_putendl_fd("Error\nmlx_new_window failed", 2), -1);
	if (create_frame(app, w, h) != 0)
		return (ft_putendl_fd("Error\nmlx_new_image failed", 2), -1);
	return (0);
}

/**
 * @brief Libère la grille de la map 2D.
 *
 * Parcourt et libère chaque ligne de la grille app->map.grid, puis libère
 * le tableau de pointeurs lui-même. Réinitialise les dimensions et le
 * pointeur à NULL/0 pour éviter les double-free.
 *
 * @param app Pointeur vers la structure principale contenant la map.
 *
 */
void free_map(t_app *app)
{
	int i;

	if (!app->map.grid)
		return;
	i = 0;
	while (i < app->map.height)
	{
		free(app->map.grid[i]);
		i++;
	}
	free(app->map.grid);
	app->map.grid = NULL;
	app->map.height = 0;
	app->map.width = 0;
}

/**
 * @brief Nettoie et libère toutes les ressources de l'application.
 *
 * Séquence de nettoyage complète dans l'ordre inverse de l'initialisation :
 * 1. Libère les textures (images MLX + chemins)
 * 2. Nettoie les buffers get_next_line
 * 3. Détruit l'image offscreen
 * 4. Détruit la fenêtre MLX
 * 5. Détruit le display MLX et libère le pointeur
 * 6. Libère la grille de map
 *
 * Peut être appelée à n'importe quel stade (gère les pointeurs NULL).
 *
 * @param app Pointeur vers la structure principale à nettoyer.
 * @param code Code de sortie (actuellement non utilisé).
 *
 */
void app_destroy(t_app *app, int code)
{
	free_textures(app);
	gnl_free(NULL);
	if (app->frame.ptr)
		mlx_destroy_image(app->mlx, app->frame.ptr);
	if (app->win)
		mlx_destroy_window(app->mlx, app->win);
	if (app->mlx)
	{
		mlx_destroy_display(app->mlx);
		free(app->mlx);
		app->mlx = NULL;
	}
	free_map(app);
	(void)code;
}
