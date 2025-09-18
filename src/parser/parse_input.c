#include "cub3d.h"
#include "libft.h"

void init_defaults(t_app *app)
{
	app->win_w = 1024;
	app->win_h = 768;
	app->move_speed = 0.06;
	app->rot_speed = 0.04;
	app->floor = (t_color){50, 50, 50};
	app->ceil = (t_color){135, 206, 235};
	app->keys = (t_keys){0};
	app->tex.loaded = false;
}

/*
 * FONCTION PARSING - Déroulement étape par étape
 * ================================================
 *
 * Cette fonction orchestre tout le processus de parsing du projet cub3D.
 * Elle suit un ordre logique pour valider et charger toutes les données nécessaires.
 *
 * ÉTAPES :
 *
 * 1. VALIDATION DES ARGUMENTS
 *    - Vérifie que l'utilisateur a donné exactement 1 fichier .cub
 *    - Vérifie que le fichier a bien l'extension .cub
 *    - Si erreur → retourne 1 (échec)
 *
 * 2. INITIALISATION DE L'APPLICATION
 *    - Remet à zéro toute la structure t_app (sécurité)
 *    - Charge les valeurs par défaut (résolution, vitesses, couleurs...)
 *    - Prépare l'app pour recevoir les données du fichier
 *
 * 3. PARSING DU FICHIER .CUB  -> parse_cub_file(app, argv[1])
 *    - Ouvre et lit le fichier ligne par ligne
 *    - Parse les textures (NO, SO, WE, EA)
 *    - Parse les couleurs (F pour sol, C pour plafond)
 *    - Parse la carte 2D (murs et espaces)
 *    - Valide que tout est cohérent
 *
 * 4. GESTION D'ERREUR
 *    - Si le parsing échoue → nettoie la mémoire et retourne 1
 *    - Si tout va bien → retourne 0 (succès)
 *
 * RETOUR :
 * - 0 : Parsing réussi, l'app est prête pour le jeu
 * - 1 : Erreur détectée, le programme doit s'arrêter
*/


int	parsing(t_app *app, int argc, char **argv)
{
	if (parse_input(app, argc, argv) == 1)
		return 1;
	// Initialiser l'app de base
	ft_memset(app, 0, sizeof(t_app));
	init_defaults(app);
	if (parse_cub_file(app, argv[1]) == 1)
	{
		app_destroy(app, 1);
		return (1);
	}
	return 0;
}
int	parse_input(t_app *app, int argc, char **argv)
{
	(void)app;
	if (argc != 2)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
		return (1);
	}
	// Verifier l'extension du fichier
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
