/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:16 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/03 11:27:14 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>
#include <string.h>

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
int	main(int argc, char **argv)
{
	t_app	app;

	// Mode "parse-only": parse, valide, cleanup, et quitte sans lancer MLX
	if (argc == 3 && strcmp(argv[1], "--parse-only") == 0)
	{
		ft_memset(&app, 0, sizeof(t_app));
		init_defaults(&app);
		if (parse_cub_file(&app, argv[2]) != 0)
		{
			app_destroy(&app, 1);
			return (1);
		}
		if (validate_map(&app) != 0)
		{
			app_destroy(&app, 1);
			return (1);
		}
		app_destroy(&app, 0);
		return (0);
	}
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
