/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/06 12:32:41 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Gère le parsing d'une ligne de définition de texture.
 *
 * Vérifie que le nombre maximum de textures (4) n'est pas dépassé, puis
 * délègue le parsing à parse_texture_line(). Incrémente le compteur de
 * textures en cas de succès. Détecte les duplications de définitions.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @param line Ligne de texture à parser (format: "NO path/to/texture.xpm").
 * @param counters Pointeur vers les compteurs de parsing.
 * @return int 0 si succès, -1 si erreur (trop de textures ou duplication).
 *
 */
int	handle_texture_line(t_app *app, char *line, t_parse_counters *counters)
{
	if (counters->texture_count == 4)
		return (error_msg("Only 4 texture paths are allowed"));
	if (parse_texture_line(app, line) != 0)
		return (-1);
	counters->texture_count++;
	return (0);
}

/**
 * @brief Gère le parsing d'une ligne de définition de couleur.
 *
 * Vérifie que le nombre maximum de couleurs (2) n'est pas dépassé, puis
 * délègue le parsing à parse_color_line(). Incrémente le compteur de
 * couleurs en cas de succès. Détecte les duplications de définitions.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @param line Ligne de couleur à parser (format: "F 220,100,0" ou "C 225,30,0").
 * @param counters Pointeur vers les compteurs de parsing.
 * @return int 0 si succès, -1 si erreur (trop de couleurs ou duplication).
 *
 */
int	handle_color_line(t_app *app, char *line, t_parse_counters *counters)
{
	if (counters->color_count == 2)
		return (error_msg("Only 2 color definitions are allowed"));
	if (parse_color_line(app, line) != 0)
		return (-1);
	counters->color_count++;
	return (0);
}

/**
 * @brief Gère le parsing d'une ligne de la map.
 *
 * Initialise la structure map lors de la première ligne de map rencontrée
 * (map_started = 1), puis ajoute chaque ligne successive à la grille via
 * add_map_line(). Incrémente le compteur de lignes de map.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @param line Ligne de map à ajouter (contient '0', '1', ' ', N/S/E/W).
 * @param counters Pointeur vers les compteurs de parsing.
 * @return int Retourne toujours 0 (succès).
 *
 */
int	handle_map_line(t_app *app, char *line, t_parse_counters *counters)
{
	if (!counters->map_started)
		counters->map_started = 1;
	add_map_line(app, line, counters->map_line_index);
	counters->map_line_index++;
	return (0);
}
