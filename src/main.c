/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:32:16 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/01 13:05:30 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include <mlx.h>

/**
 * @brief Application entry point.
 *
 * Parses command-line arguments and map file, initializes the application and
 * MiniLibX, registers input and loop hooks, starts the MLX event loop, and
 * cleans up resources on exit.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return int Returns 0 on success, non-zero on failure.
 */
int	main(int argc, char **argv)
{
	t_app	app;

	if (parsing(&app, argc, argv) != 0)
		return (1);
	if (app_init(&app, app.win_w, app.win_h) != 0)
	{
		app_destroy(&app, 1);
		return (1);
	}
	mlx_loop_hook(app.mlx, (int (*)(void *))app_loop, &app);
	mlx_hook(app.win, 2, 1L << 0, key_press, &app);
	mlx_hook(app.win, 3, 1L << 1, key_release, &app);
	mlx_hook(app.win, 17, 0, close_window, &app);
	mlx_loop(app.mlx);
	app_destroy(&app, 0);
	return (0);
}
