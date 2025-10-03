/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 05:20:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/03 14:55:15 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Vérifie si une ligne est vide ou ne contient qu'un retour
 *        à la ligne.
 *
 * Utilisé pour filtrer les lignes vides lors du parsing du fichier .cub.
 * Une ligne vide au milieu de la section map est considérée comme une
 * erreur et doit être rejetée.
 *
 * @param line Ligne à vérifier.
 * @return int Retourne 1 si la ligne est vide, 0 sinon.
 *
 */
int	is_empty_line(char *line)
{
	return (ft_strlen(line) == 0 || line[0] == '\n');
}

/**
 * @brief Valide que tous les éléments requis ont été parsés.
 *
 * Vérifie que le fichier .cub contient bien toutes les textures,
 * couleurs et une carte. Affiche des messages d'erreur spécifiques
 * selon l'élément manquant.
 *
 * Cette validation est effectuée une fois que tout le fichier a été lu.
 *
 * @param app Pointeur vers la structure de l'application.
 * @param counters Structure contenant les compteurs de parsing.
 * @return int 0 si tous les éléments requis sont présents, -1 sinon.
 *
 */
int	validate_parsing_completion(t_app *app, t_parse_counters *counters)
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
