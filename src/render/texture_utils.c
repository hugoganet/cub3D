/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:45:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 14:45:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

/**
 * @brief Affiche un message d'erreur pour le chargement de texture
 *
 * Écrit sur la sortie d'erreur standard un message formaté avec
 * le chemin de la texture problématique.
 *
 * @param path Chemin du fichier XPM défaillant
 * @param message Message d'erreur descriptif
 * @see load_single_texture
 */
static void	print_texture_error(char *path, char *message)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	ft_putendl_fd(path, 2);
}

/**
 * @brief Charge un fichier XPM en tant qu'image MLX
 *
 * Utilise mlx_xpm_file_to_image pour charger une texture depuis un
 * fichier XPM. Récupère les dimensions (largeur/hauteur) et stocke
 * le pointeur d'image.
 *
 * @param app Structure principale de l'application
 * @param path Chemin du fichier XPM
 * @param texture Structure d'image à remplir
 * @return int 0 si succès, 1 si échec
 * @see load_single_texture
 */
static int	load_texture_image(t_app *app, char *path, t_img *texture)
{
	int	width;
	int	height;

	texture->ptr = mlx_xpm_file_to_image(app->mlx, path, &width, &height);
	if (!texture->ptr)
	{
		print_texture_error(path, "Failed to load texture: ");
		return (1);
	}
	texture->w = width;
	texture->h = height;
	return (0);
}

/**
 * @brief Récupère le pointeur vers les données pixels de la texture
 *
 * Utilise mlx_get_data_addr pour obtenir l'accès direct aux pixels
 * de la texture. Extrait aussi les métadonnées (bpp, line_len, endian).
 *
 * @param app Structure principale de l'application
 * @param path Chemin de la texture (pour les messages d'erreur)
 * @param texture Structure d'image contenant le pointeur MLX
 * @return int 0 si succès, 1 si échec
 * @see load_single_texture
 */
static int	get_texture_data(t_app *app, char *path, t_img *texture)
{
	texture->addr = mlx_get_data_addr(texture->ptr, &texture->bpp,
			&texture->line_len, &texture->endian);
	if (!texture->addr)
	{
		print_texture_error(path, "Failed to get texture data: ");
		mlx_destroy_image(app->mlx, texture->ptr);
		texture->ptr = NULL;
		return (1);
	}
	return (0);
}

/**
 * @brief Charge complètement une texture depuis un fichier XPM
 *
 * Fonction principale pour charger une texture : initialise la structure,
 * charge l'image MLX et récupère l'accès aux données pixels. Gère les
 * erreurs à chaque étape.
 *
 * @param app Structure principale de l'application
 * @param path Chemin du fichier XPM à charger
 * @param texture Pointeur vers la structure d'image à initialiser
 * @return int 0 si succès, 1 si échec
 * @see load_textures
 * @see load_texture_image
 * @see get_texture_data
 */
int	load_single_texture(t_app *app, char *path, t_img *texture)
{
	texture->ptr = NULL;
	texture->addr = NULL;
	texture->w = 0;
	texture->h = 0;
	if (load_texture_image(app, path, texture))
		return (1);
	if (get_texture_data(app, path, texture))
		return (1);
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
 * @see draw_textured_column
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
