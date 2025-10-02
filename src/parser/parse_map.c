/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:02:24 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/02 13:06:50 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Initialise la structure map à des valeurs par défaut.
 *
 * Prépare la structure app->map avant le parsing en définissant tous
 * les champs à zéro ou NULL. La grille sera allouée dynamiquement
 * lors de l'ajout des lignes via add_map_line().
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @return int Retourne toujours 0 (succès).
 *
 */
int init_map(t_app *app)
{
	app->map.height = 0;
	app->map.width = 0;
	app->map.grid = NULL;
	return (0);
}

/**
 * @brief Calcule la longueur effective d'une ligne sans le retour chariot.
 *
 * Détermine la longueur de la chaîne en excluant le caractère '\n' final
 * s'il est présent. Utile pour normaliser les lignes de map avant copie.
 *
 * @param line Chaîne à mesurer (peut être NULL).
 * @return int Longueur de la ligne sans '\n', ou 0 si line est NULL.
 */
int get_trimmed_len(char *line)
{
	int len;

	if (!line)
		return (0);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;
	return (len);
}

/**
 * @brief Duplique une ligne de map dans une allocation propre.
 *
 * Alloue un buffer de taille len+1 et copie exactement len caractères
 * depuis line. Termine la chaîne par '\0'. En cas d'échec d'allocation,
 * appelle error_exit() pour arrêter le programme proprement.
 *
 * @param app Pointeur vers la structure principale (pour error_exit).
 * @param line Chaîne source à dupliquer.
 * @param len Nombre de caractères à copier (excluant '\0').
 * @return char* Pointeur vers la nouvelle chaîne allouée.
 *
 */
char *dup_map_line(t_app *app, char *line, int len)
{
	char *map_line;
	int i;

	map_line = malloc(len + 1);
	if (!map_line)
		error_exit(app, "Memory allocation failed for map line");
	i = 0;
	while (i < len)
	{
		map_line[i] = line[i];
		i++;
	}
	map_line[i] = '\0';
	return (map_line);
}

/**
 * @brief Réalloue la grille de map pour garantir la capacité nécessaire.
 *
 * Utilise realloc() pour agrandir le tableau de pointeurs app->map.grid
 * afin qu'il puisse contenir au moins 'needed' entrées. En cas d'échec,
 * appelle error_exit() pour terminer proprement.
 *
 * @param app Pointeur vers la structure principale (pour error_exit).
 * @param old_grid Tableau actuel de pointeurs (peut être NULL au départ).
 * @param needed Nombre d'entrées minimum requises dans le tableau.
 * @return char** Pointeur vers le tableau réalloué.
 *
 */
char **ensure_grid_capacity(t_app *app, char **old_grid, int needed)
{
	char **new_grid;

	new_grid = realloc(old_grid, sizeof(char *) * needed);
	if (!new_grid)
		error_exit(app, "Memory reallocation failed for map");
	return (new_grid);
}

/**
 * @brief Ajoute une ligne formatée à la grille de la map.
 *
 * Cette version refactorisée utilise des helpers pour clarifier les étapes :
 * - get_trimmed_len() : calcule la longueur effective sans '\n'
 * - dup_map_line() : duplique la ligne dans une allocation propre
 * - ensure_grid_capacity() : réalloue le tableau app->map.grid si nécessaire
 *
 * La fonction insère la nouvelle ligne à l'index spécifié, termine la grille
 * par NULL, met à jour app->map.height et adapte app->map.width si la ligne
 * est plus large que la largeur courante.
 *
 * En cas d'allocation échouée, les helpers appellent error_exit() (arrêt).
 *
 * @param app Pointeur vers la structure principale contenant app->map.
 * @param line Chaîne à ajouter (peut contenir un '\n' final).
 * @param line_index Index 0-based où insérer la ligne dans la grille.
 * @return int 0 si succès, 1 si line == NULL (erreur non-fatale).
 */
int add_map_line(t_app *app, char *line, int line_index)
{
	int len;
	char *map_line;
	char **new_grid;

	if (!line)
		return (1);
	len = get_trimmed_len(line);
	map_line = dup_map_line(app, line, len);
	new_grid = ensure_grid_capacity(app, app->map.grid, line_index + 2);
	app->map.grid = new_grid;
	app->map.grid[line_index] = map_line;
	app->map.grid[line_index + 1] = NULL;
	app->map.height = line_index + 1;
	if (len > app->map.width)
		app->map.width = len;
	return (0);
}
