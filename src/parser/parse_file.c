/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:57:49 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/02 13:06:49 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Parse une ligne du fichier .cub et route vers le handler approprié.
 *
 * Analyse le type de ligne (texture, couleur, ou map) et délègue le traitement
 * au handler spécialisé. Maintient l'ordre strict du format .cub :
 * textures/couleurs d'abord, puis map.
 * Toute ligne non reconnue génère une erreur.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @param line Ligne à parser (peut contenir '\n' final).
 * @param counters Pointeur vers les compteurs de parsing (texture/color/map).
 * @return int 0 si succès, -1 si erreur de parsing ou format invalide.
 *
 */
int parse_single_line(t_app *app, char *line, t_parse_counters *counters)
{
	if (is_texture_line(line) && !counters->map_started)
		return (handle_texture_line(app, line, counters));
	else if (is_color_line(line) && !counters->map_started)
	{
		if (parse_color_line(app, line) != 0)
			return (-1);
		counters->color_count++;
	}
	else if (is_map_line(line))
		return (handle_map_line(app, line, counters));
	else
	{
		ft_putendl_fd("Error", 2);
		ft_putstr_fd("Unknown line format: ", 2);
		ft_putstr_fd(line, 2);
		return (-1);
	}
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
		free(line);
		gnl_free(NULL);
		close(fd);
		app_destroy(app, 1);
		exit(1);
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
static int validate_parsing_completion(t_app *app,
										t_parse_counters *counters)
{
	(void)app;
	if (counters->texture_count != 4)
		return (error_msg("Missing texture definitions (need NO, SO, WE, EA)"));
	if (counters->color_count != 2)
		return (error_msg("Missing color definitions (need F and C)"));
	if (!counters->map_started)
		return (error_msg("No map found in file"));
	return (0);
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
 */
int parse_cub_file(t_app *app, const char *path)
{
	t_parse_counters counters;
	int fd;
	char *line;
	int result;

	counters = (t_parse_counters){0};
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (error_msg("Cannot open file"));
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
	if (validate_parsing_completion(app, &counters) != 0)
		return (-1);
	return (0);
}
