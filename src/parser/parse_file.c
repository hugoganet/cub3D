#include "cub3d.h"
#include "libft.h"

/**
 * @brief Analyse une ligne individuelle du fichier .cub.
 *
 * Détermine le type de ligne (texture, couleur, carte) et appelle la
 * fonction de parsing appropriée. Met à jour les compteurs pour suivre
 * la progression du parsing. Gère l'ordre strict : textures/couleurs
 * avant la carte.
 *
 * @param app Pointeur vers la structure de l'application.
 * @param line Ligne à analyser (sans retour à la ligne).
 * @param counters Pointeur vers les compteurs de parsing.
 * @return int Retourne 0 en cas de succès, -1 en cas d'erreur.
 */
int parse_single_line(t_app *app, char *line, t_parse_counters *counters)
{
	if (is_texture_line(line) && !counters->map_started)
	{
		parse_texture_line(app, line);
		counters->texture_count++;
	}
	else if (is_color_line(line) && !counters->map_started)
	{
		if (parse_color_line(app, line) != 0)
			return (-1);
		counters->color_count++;
	}
	else if (is_map_line(line))
	{
		if (!counters->map_started)
		{
			counters->map_started = 1;
			init_map(app);
		}
		add_map_line(app, line, counters->map_line_index);
		counters->map_line_index++;
	}
	else
		return (printf("Error:\nUnknown line format: %s", line), -1);
	return (0);
}

/**
 * @brief Vérifie si une ligne est vide ou ne contient qu'un retour
 *        à la ligne.
 *
 * @param line Ligne à vérifier.
 * @return int Retourne 1 si la ligne est vide, 0 sinon.
 */
int is_empty_line(char *line)
{
	return (ft_strlen(line) == 0 || line[0] == '\n');
}

/**
 * @brief Traite une ligne du fichier .cub avec gestion d'erreurs.
 *
 * Fonction helper qui encapsule la logique de traitement d'une ligne :
 * ignore les lignes vides, parse les lignes valides, et gère le
 * nettoyage mémoire en cas d'erreur.
 *
 * @param app Pointeur vers la structure de l'application.
 * @param line Ligne à traiter.
 * @param counters Pointeur vers les compteurs de parsing.
 * @param fd Descripteur de fichier pour fermeture en cas d'erreur.
 * @return int Retourne 0 pour continuer, 1 pour ligne vide, -1 erreur.
 */
static int process_line(t_app *app, char *line, t_parse_counters *counters,
						int fd)
{
	if (is_empty_line(line))
		return (1);
	if (parse_single_line(app, line, counters) != 0)
	{
		gnl_free(NULL);
		close(fd);
		error_exit(app, "Invalid RGB values (must be 0-255)");
	}
	return (0);
}

/**
 * @brief Valide que tous les éléments requis ont été parsés.
 *
 * Vérifie que le fichier .cub contient bien toutes les textures,
 * couleurs et une carte. Affiche des messages d'erreur spécifiques
 * selon l'élément manquant.
 *
 * @param app Pointeur vers la structure de l'application.
 * @param counters Structure contenant les compteurs de parsing.
 */
static void validate_parsing_completion(t_app *app,
										t_parse_counters *counters)
{
	if (counters->texture_count != 4)
		error_exit(app, "Missing texture definitions (need NO, SO, WE, EA)");
	if (counters->color_count != 2)
		error_exit(app, "Missing color definitions (need F and C)");
	if (!counters->map_started)
		error_exit(app, "No map found in file");
}

/**
 * @brief Parse un fichier .cub et charge toutes les données de
 *        configuration.
 *
 * Lit et analyse un fichier .cub ligne par ligne pour extraire textures,
 * couleurs et carte. Suit un ordre strict : textures/couleurs puis carte.
 * Valide que tous les éléments requis sont présents.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @param path Chemin vers le fichier .cub à parser.
 * @return int Retourne 0 en cas de succès.
 *
 * @see process_line() pour le traitement des lignes
 * @see validate_parsing_completion() pour la validation finale
 */
int parse_cub_file(t_app *app, const char *path)
{
	int fd;
	char *line;
	t_parse_counters counters;
	int result;

	counters = (t_parse_counters){0};

	fd = open(path, O_RDONLY);
	if (fd < 0)
		error_exit(app, "Cannot open file");
	line = get_next_line(fd);
	while (line != NULL)
	{
		result = process_line(app, line, &counters, fd);
		free(line);
		if (result == 1)
		{
			line = get_next_line(fd);
			continue;
		}
		line = get_next_line(fd);
	}
	gnl_free(NULL);
	close(fd);
	validate_parsing_completion(app, &counters);
	return (0);
}
