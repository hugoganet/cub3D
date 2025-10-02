/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:16 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/02 13:06:49 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

/**
 * @brief Point d'entrée de l'application cub3D.
 *
 * Orchestre le démarrage complet du programme :
 * 1. Parse les arguments et le fichier .cub
 * 2. Initialise MiniLibX, la fenêtre et les textures
 * 3. Enregistre les hooks d'événements (clavier, boucle, fermeture)
 * 4. Lance la boucle d'événements MLX (bloquante)
 * 5. Nettoie les ressources à la sortie
 *
 * @param argc Nombre d'arguments de ligne de commande.
 * @param argv Tableau des arguments de ligne de commande.
 * @return int Retourne 0 en cas de succès, 1 en cas d'erreur.
 *
 */
int main(int argc, char **argv)
{
	t_app app;

	if (parsing(&app, argc, argv) != 0)
		return (1);
	if (app_init(&app, app.win_w, app.win_h) != 0)
	{
		app_destroy(&app, 1);
		return (1);
	}
	mlx_loop_hook(app.mlx, (int (*)(void *))app_loop, &app);
	// ? C'est quoi cette syntax `1L << 0` ?
	mlx_hook(app.win, 2, 1L << 0, key_press, &app);
	mlx_hook(app.win, 3, 1L << 1, key_release, &app);
	mlx_hook(app.win, 17, 0, close_window, &app);
	mlx_loop(app.mlx);
	// ? dead code ⬇️
	app_destroy(&app, 0);
	return (0);
}
