/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:50:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/01 18:26:28 by hugoganet        ###   ########.fr       */
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
