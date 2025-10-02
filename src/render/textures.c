/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:33:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 13:33:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

/**
 * @brief Charge les 4 textures de murs (NO/SO/WE/EA)
 *
 * Charge séquentiellement les textures des 4 directions en utilisant
 * les chemins définis dans le fichier .cub. En cas d'erreur sur une
 * texture, libère celles déjà chargées avant de retourner une erreur.
 *
 * @param app Structure principale de l'application
 * @return int 0 si succès, 1 si échec
 * @see load_single_texture
 * @see free_textures
 */
int	load_textures(t_app *app)
{
	if (load_single_texture(app, app->tex.north_path, &app->tex.north))
		return (1);
	if (load_single_texture(app, app->tex.south_path, &app->tex.south))
	{
		free_textures(app);
		return (1);
	}
	if (load_single_texture(app, app->tex.west_path, &app->tex.west))
	{
		free_textures(app);
		return (1);
	}
	if (load_single_texture(app, app->tex.east_path, &app->tex.east))
	{
		free_textures(app);
		return (1);
	}
	app->tex.loaded = true;
	return (0);
}

/**
 * @brief Sélectionne la texture appropriée selon la face du mur touchée
 *
 * Détermine quelle texture de mur utiliser en fonction du côté de
 * collision (vertical/horizontal) et de la direction du rayon :
 * - Side 0 (vertical) : WE si ray.x > 0, sinon EA
 * - Side 1 (horizontal) : SO si ray.y > 0, sinon NO
 *
 * @param app Structure principale de l'application
 * @param side Côté de collision (0 = vertical, 1 = horizontal)
 * @param ray_dir Direction du rayon
 * @return t_img* Pointeur vers la texture appropriée
 * @see draw_textured_column
 * @see perform_dda
 */
t_img	*get_wall_texture(t_app *app, int side, t_vec2 ray_dir)
{
	if (side == 0)
	{
		if (ray_dir.x > 0)
			return (&app->tex.east);
		else
			return (&app->tex.west);
	}
	else
	{
		if (ray_dir.y > 0)
			return (&app->tex.south);
		else
			return (&app->tex.north);
	}
}
