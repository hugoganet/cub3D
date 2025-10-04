/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:02:06 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/04 20:10:28 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Fonction principale de parsing qui orchestre la validation et
 *        l'analyse du fichier.
 *
 * Valide les arguments de ligne de commande, initialise la structure de
 * l'application avec des valeurs par défaut, analyse le fichier .cub, et
 * valide la carte résultante. Nettoie les ressources en cas d'échec.
 *
 * @param app Pointeur vers la structure de l'application à remplir.
 * @param argc Nombre d'arguments de ligne de commande.
 * @param argv Tableau des chaînes d'arguments de ligne de commande.
 * @return int Retourne 0 en cas de succès, 1 en cas d'échec.
 */
int	parsing(t_app *app, int argc, char **argv)
{
	if (parse_input(argc, argv) != 0)
		return (-1);
	ft_memset(app, 0, sizeof(t_app));
	init_defaults(app);
	if (parse_cub_file(app, argv[1]) != 0)
	{
		app_destroy(app);
		return (-1);
	}
	if (validate_map(app) != 0)
	{
		app_destroy(app);
		return (-1);
	}
	return (0);
}

/**
 * @brief Initialise la structure de l'application avec des valeurs par
 *        défaut.
 *
 * Configure les dimensions par défaut de la fenêtre, les vitesses de
 * mouvement et de rotation, les couleurs par défaut du sol et du plafond,
 * remet à zéro l'état des touches, et initialise les pointeurs de texture
 * et chemins à NULL/false.
 *
 * @param app Pointeur vers la structure de l'application à initialiser.
 */
void	init_defaults(t_app *app)
{
	app->win_w = DEFAULT_WIN_WIDTH;
	app->win_h = DEFAULT_WIN_HEIGHT;
	app->move_speed = MOVE_SPEED;
	app->rot_speed = ROT_SPEED;
	app->floor = (t_color){50, 50, 50};
	app->ceil = (t_color){135, 206, 235};
	app->keys = (t_keys){0};
	app->tex.loaded = false;
	app->tex.north.ptr = NULL;
	app->tex.south.ptr = NULL;
	app->tex.west.ptr = NULL;
	app->tex.east.ptr = NULL;
	app->tex.north_path = NULL;
	app->tex.south_path = NULL;
	app->tex.west_path = NULL;
	app->tex.east_path = NULL;
}

/**
 * @brief Valide les arguments de ligne de commande et l'extension du
 *        fichier.
 *
 * Vérifie qu'exactement un argument est fourni, que le chemin du fichier
 * est valide (au moins 5 caractères), et qu'il a une extension .cub.
 * Affiche des messages d'erreur appropriés sur stderr en cas d'échec de
 * validation.
 *
 * @param argc Nombre d'arguments de ligne de commande.
 * @param argv Tableau des chaînes d'arguments de ligne de commande.
 * @return int Retourne 0 en cas de succès, 1 en cas d'échec de validation.
 */
int	parse_input(int argc, char **argv)
{
	char	*filepath;

	filepath = argv[1];
	if (argc != 2)
		return (error_msg("Usage: ./cub3D <map.cub>"));
	if (!filepath || ft_strlen(filepath) < 5)
		return (error_msg("Invalid file path"));
	if (ft_strncmp(filepath + ft_strlen(filepath) - 4, ".cub", 4) != 0)
		return (error_msg("File must have .cub extension"));
	return (0);
}
