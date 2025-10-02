/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:24 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/02 11:09:11 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include <stdlib.h>

/**
 * @brief Affiche un msg d'erreur, clean les ressources et termine le programme.
 *
 * Fonction centrale de gestion d'erreurs qui :
 * 1. Affiche "Error" sur stderr
 * 2. Affiche le message d'erreur optionnel sur stderr
 * 3. Appelle app_destroy() pour libérer toutes les ressources
 * 4. Termine le programme avec exit(1)
 *
 * Cette fonction ne retourne jamais (attribut noreturn implicite).
 *
 * @param app Pointeur vers la structure principale à nettoyer.
 * @param msg Message d'erreur descriptif à afficher (peut être NULL).
 *
 */
void	error_exit(t_app *app, const char *msg)
{
	ft_putendl_fd("Error", 2);
	if (msg)
		ft_putendl_fd((char *)msg, 2);
	app_destroy(app, 1);
	exit(1);
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
