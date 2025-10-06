/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:45:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/06 15:06:08 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

/**
 * @brief Charge complètement une texture depuis un fichier XPM
 *
 * Charge l'image XPM avec MLX, récupère ses dimensions et l'accès
 * aux données pixels en une seule opération.
 *
 * @param app Structure principale de l'application
 * @param path Chemin du fichier XPM à charger
 * @param texture Pointeur vers la structure d'image à initialiser
 * @return int 0 si succès, -1 si échec
 */
int	load_single_texture(t_app *app, char *path, t_img *texture)
{
    int	width;
    int	height;

    texture->ptr = mlx_xpm_file_to_image(app->mlx, path, &width, &height);
    if (!texture->ptr)
        return (-1);
    texture->w = width;
    texture->h = height;
    texture->addr = mlx_get_data_addr(texture->ptr, &texture->bpp,
            &texture->line_len, &texture->endian);
    if (!texture->addr)
    {
        mlx_destroy_image(app->mlx, texture->ptr);
        texture->ptr = NULL;
        return (error_msg("Failed to get texture data"));
    }
    return (0);
}

/**
 * @brief Récupère la couleur d'un pixel dans une texture
 *
 * Accède directement au buffer de pixels de la texture pour extraire
 * la couleur à la position (x, y). Inclut une vérification des limites.
 * Retourne noir (0x000000) si hors-limites.
 *
 * @param texture Pointeur vers la structure de texture
 * @param x Coordonnée X du pixel (0 à texture->w-1)
 * @param y Coordonnée Y du pixel (0 à texture->h-1)
 * @return int Couleur du pixel en format hexadécimal (masque 0xFFFFFF)
 */
int	get_texture_pixel(t_img *texture, int x, int y)
{
	char	*pixel;
	int		color;

	if (x < 0 || x >= texture->w || y < 0 || y >= texture->h)
		return (0x000000);
	pixel = texture->addr + (y * texture->line_len + x * (texture->bpp / 8));
	color = *(int *)pixel;
	return (color & 0xFFFFFF);
}
