/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:57:49 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/03 14:56:30 by hugoganet        ###   ########.fr       */
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
int	parse_single_line(t_app *app, char *line, t_parse_counters *counters)
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
static int	process_line(t_app *app, char *line, t_parse_counters *counters,
						int fd)
{
	if (is_empty_line(line))
	{
		if (counters->map_started)
		{
			(void)fd;
			return (error_msg("Empty line inside map section"));
		}
		return (1);
	}
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
 * @brief Lit et traite toutes les lignes d'un fichier ouvert.
 *
 * Boucle de lecture principale qui lit chaque ligne avec get_next_line,
 * la traite via process_line, et gère la continuation ou l'arrêt selon
 * le résultat. Libère la mémoire et ferme le descripteur en cas d'erreur.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @param fd Descripteur du fichier ouvert.
 * @param counters Pointeur vers les compteurs de parsing.
 * @return int 0 si succès, -1 si erreur de traitement.
 *
 */
static int	read_and_process_lines(t_app *app, int fd,
								t_parse_counters *counters)
{
	char	*line;
	int		result;

	line = get_next_line(fd);
	while (line != NULL)
	{
		result = process_line(app, line, counters, fd);
		free(line);
		if (result == 1)
		{
			line = get_next_line(fd);
			continue ;
		}
		if (result == -1)
		{
			gnl_free(NULL);
			close(fd);
			return (-1);
		}
		line = get_next_line(fd);
	}
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
int	parse_cub_file(t_app *app, const char *path)
{
	t_parse_counters	counters;
	int					fd;

	counters = (t_parse_counters){0};
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (error_msg("Cannot open file"));
	if (read_and_process_lines(app, fd, &counters) != 0)
		return (-1);
	gnl_free(NULL);
	close(fd);
	if (validate_parsing_completion(app, &counters) != 0)
		return (-1);
	return (0);
}
