#include "cub3d.h"
#include "libft.h"

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
	if (parse_input(argc, argv) == 1)
		return (1);
	ft_memset(app, 0, sizeof(t_app));
	init_defaults(app);
	if (parse_cub_file(app, argv[1]) == 1)
	{
		app_destroy(app, 1);
		return (1);
	}
	validate_map(app);
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
	if (argc != 2)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
		return (1);
	}
	if (!argv[1] || ft_strlen(argv[1]) < 5)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("Invalid file path", 2);
		return (1);
	}
	if (ft_strncmp(argv[1] + ft_strlen(argv[1]) - 4, ".cub", 4) != 0)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("File must have .cub extension", 2);
		return (1);
	}
	return (0);
}
