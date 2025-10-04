/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 18:30:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/04 18:30:00 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

/**
 * @brief Libère la mémoire des chemins de textures
 *
 * Libère les 4 chaînes de caractères contenant les chemins vers les
 * fichiers XPM des textures (NO/SO/EA/WE). Remet les pointeurs à NULL
 * après libération pour éviter les double-free.
 *
 * @param app Structure principale de l'application
 */
static void	free_texture_paths(t_app *app)
{
	if (app->tex.north_path)
	{
		free(app->tex.north_path);
		app->tex.north_path = NULL;
	}
	if (app->tex.south_path)
	{
		free(app->tex.south_path);
		app->tex.south_path = NULL;
	}
	if (app->tex.east_path)
	{
		free(app->tex.east_path);
		app->tex.east_path = NULL;
	}
	if (app->tex.west_path)
	{
		free(app->tex.west_path);
		app->tex.west_path = NULL;
	}
}

/**
 * @brief Libère toutes les ressources liées aux textures
 *
 * Détruit les 4 images MLX des textures de murs, libère les chemins
 * de fichiers et remet le flag loaded à false. Fonction appelée lors
 * du cleanup général ou en cas d'erreur de chargement.
 *
 * @param app Structure principale de l'application
 */
void	free_textures(t_app *app)
{
	if (app->tex.north.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.north.ptr);
		app->tex.north.ptr = NULL;
	}
	if (app->tex.south.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.south.ptr);
		app->tex.south.ptr = NULL;
	}
	if (app->tex.west.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.west.ptr);
		app->tex.west.ptr = NULL;
	}
	if (app->tex.east.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.east.ptr);
		app->tex.east.ptr = NULL;
	}
	free_texture_paths(app);
	app->tex.loaded = false;
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
void	free_map(t_app *app)
{
	int	i;

	if (!app->map.grid)
		return ;
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
 *
 */
void	app_destroy(t_app *app)
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
}
