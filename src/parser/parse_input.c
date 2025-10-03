/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:02:06 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/03 14:37:19 by hugoganet        ###   ########.fr       */
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
		app_destroy(app, 1);
		return (-1);
	}
	if (validate_map(app) != 0)
	{
		app_destroy(app, 1);
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
	app->win_w = 1024;
	app->win_h = 768;
	app->move_speed = 0.03;
	app->rot_speed = 0.03;
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
 * @brief Valide le chemin du fichier et son extension.
 *
 * Vérifie que le chemin du fichier est valide (au moins 5 caractères) et
 * qu'il a une extension .cub. Affiche des messages d'erreur appropriés
 * sur stderr en cas d'échec de validation.
 *
 * @param filepath Chemin du fichier à valider.
 * @return int Retourne 0 en cas de succès, -1 en cas d'échec de validation.
 */
static int	validate_file_path(char *filepath)
{
	if (!filepath || ft_strlen(filepath) < 5)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("Invalid file path", 2);
		return (-1);
	}
	if (ft_strncmp(filepath + ft_strlen(filepath) - 4, ".cub", 4) != 0)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("File must have .cub extension", 2);
		return (-1);
	}
	return (0);
}

/**
 * @brief Gère le mode parse-only avec l'option --parse-only.
 *
 * Valide que le mode parse-only est correctement invoqué avec un fichier
 * .cub valide.
 *
 * @param argv Tableau des chaînes d'arguments de ligne de commande.
 * @return int Retourne 0 en cas de succès, -1 en cas d'échec de validation.
 */
static int	handle_parse_only_mode(char **argv)
{
	return (validate_file_path(argv[2]));
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
	if (argc == 3 && ft_strncmp(argv[1], "--parse-only", 12) == 0)
		return (handle_parse_only_mode(argv));
	if (argc != 2)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("Usage:", 2);
		ft_putendl_fd("  ./cub3D <map.cub>", 2);
		ft_putendl_fd("  ./cub3D --parse-only <map.cub>", 2);
		return (-1);
	}
	return (validate_file_path(argv[1]));
}
