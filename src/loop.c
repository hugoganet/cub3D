/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:12 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/04 12:05:59 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>
#include <math.h>

/**
 * @brief Boucle principale de rendu du jeu (appelée à chaque frame).
 *
 * Pipeline de rendu complet exécuté à chaque itération de mlx_loop :
 * 1. Mise à jour de la position/rotation du joueur selon les touches
 * 2. Rendu du fond (plafond + sol avec couleurs unies)
 * 3. Rendu de la vue 3D (raycasting + murs texturés)
 * 4. Rendu de la minimap overlay (position + rayons)
 * 5. Affichage du buffer à l'écran (double buffering)
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @return int Retourne toujours 0 (requis par MLX).
 *
 */
int	app_loop(t_app *app)
{
	update_player_movement(app);
	render_background(app);
	render_3d_view(app);
	render_minimap(app);
	mlx_put_image_to_window(app->mlx, app->win, app->frame.ptr, 0, 0);
	return (0);
}

/**
 * @brief Gestionnaire de fermeture de fenêtre (croix rouge ou ESC).
 *
 * Fonction de callback appelée quand l'utilisateur ferme la fenêtre
 * (clic sur la croix ou touche ESC). Nettoie toutes les ressources via
 * app_destroy() puis termine le programme avec exit(0).
 *
 * Cette fonction ne retourne jamais (exit).
 *
 * @param app Pointeur vers la structure principale à nettoyer.
 * @return int Jamais atteint (exit avant), mais retournerait 0.
 *
 */
int	close_window(t_app *app)
{
	app_destroy(app);
	exit(0);
	return (0);
}
