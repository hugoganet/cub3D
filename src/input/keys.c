/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 15:20:01 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 15:20:02 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Gestionnaire d'événements pour les touches pressées.
 *
 * Fonction de callback appelée par MiniLibX lorsqu'une touche est
 * pressée. Met à jour l'état des touches dans la structure de
 * l'application pour permettre la gestion de touches multiples
 * simultanées. Gère également la touche d'échappement pour quitter.
 *
 * @param keycode Code de la touche pressée (défini dans cub3d.h).
 * @param app Pointeur vers la structure de l'application.
 * @return int Retourne toujours 0 (requis par MiniLibX).
 */
int	key_press(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		close_window(app);
	else if (keycode == KEY_W)
		app->keys.w = true;
	else if (keycode == KEY_A)
		app->keys.a = true;
	else if (keycode == KEY_S)
		app->keys.s = true;
	else if (keycode == KEY_D)
		app->keys.d = true;
	else if (keycode == KEY_LEFT)
		app->keys.left = true;
	else if (keycode == KEY_RIGHT)
		app->keys.right = true;
	return (0);
}

/**
 * @brief Gestionnaire d'événements pour les touches relâchées.
 *
 * Fonction de callback appelée par MiniLibX lorsqu'une touche est
 * relâchée. Remet à false l'état de la touche correspondante dans
 * la structure de l'application, permettant d'arrêter le mouvement
 * ou la rotation du joueur.
 *
 * @param keycode Code de la touche relâchée (défini dans cub3d.h).
 * @param app Pointeur vers la structure de l'application.
 * @return int Retourne toujours 0 (requis par MiniLibX).
 */
int	key_release(int keycode, t_app *app)
{
	if (keycode == KEY_W)
		app->keys.w = false;
	else if (keycode == KEY_A)
		app->keys.a = false;
	else if (keycode == KEY_S)
		app->keys.s = false;
	else if (keycode == KEY_D)
		app->keys.d = false;
	else if (keycode == KEY_LEFT)
		app->keys.left = false;
	else if (keycode == KEY_RIGHT)
		app->keys.right = false;
	return (0);
}
