/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/03 05:10:36 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Affiche un message d'erreur sur stderr et retourne -1.
 *
 * Fonction helper pour gérer les erreurs sans terminer le programme.
 * Affiche "Error" suivi du message spécifique sur stderr, puis retourne -1
 * pour permettre la propagation de l'erreur vers les couches supérieures.
 *
 * @param msg Message d'erreur descriptif (peut être NULL).
 * @return int Retourne toujours -1.
 */
int	error_msg(const char *msg)
{
	ft_putendl_fd("Error", 2);
	if (msg)
		ft_putendl_fd((char *)msg, 2);
	return (-1);
}

/**
 * @brief Convertit une structure t_color en entier RGB au format MLX.
 *
 * Transforme les composantes RGB (0-255) en un entier 32 bits au format :
 * 0xAARRGGBB avec alpha = 0xFF (opaque). Clamp automatiquement chaque
 * composante à la plage [0,255] pour éviter les dépassements.
 *
 * Format de sortie : (0xFF << 24) | (R << 16) | (G << 8) | B
 *
 * @param c Structure t_color contenant les composantes r, g, b.
 * @return int Couleur encodée en 0xAARRGGBB pour MLX.
 *
 */
int	rgb_to_int(t_color c)
{
	if (c.r < 0)
		c.r = 0;
	if (c.r > 255)
		c.r = 255;
	if (c.g < 0)
		c.g = 0;
	if (c.g > 255)
		c.g = 255;
	if (c.b < 0)
		c.b = 0;
	if (c.b > 255)
		c.b = 255;
	return ((0xFF << 24) | (c.r << 16) | (c.g << 8) | c.b);
}
