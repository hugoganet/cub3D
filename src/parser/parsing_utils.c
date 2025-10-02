/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:22:58 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/02 13:06:50 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Réinitialise les champs de map après parsing des en-têtes.
 *
 * Prépare la structure app->map pour le parsing des lignes de carte
 * en réinitialisant tous les champs à leurs valeurs par défaut.
 * Appelé après avoir terminé le parsing des textures et couleurs.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @return int Retourne toujours 0 (succès).
 *
 */
int set_defaults_after_parse(t_app *app)
{
	app->map.grid = NULL;
	app->map.width = 0;
	app->map.height = 0;
	return (0);
}

/**
 * @brief Détecte si une ligne définit une texture.
 *
 * Vérifie si la ligne commence par un des préfixes de texture valides :
 * "NO ", "SO ", "WE ", "EA " (suivis d'un espace). Utilisé pour
 * identifier les lignes de configuration de textures murales.
 *
 * @param line Chaîne à analyser.
 * @return int 1 si la ligne est une déclaration de texture, 0 sinon.
 *
 */
int is_texture_line(char *line)
{
	if (!line || ft_strlen(line) < 3)
		return (0);
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0 || ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
		return (1);
	return (0);
}

/**
 * @brief Détecte si une ligne définit une couleur.
 *
 * Vérifie si la ligne commence par "F " (floor/sol) ou "C " (ceiling/plafond).
 * Utilisé pour identifier les lignes de configuration des couleurs de
 * fond (sol et plafond).
 *
 * @param line Chaîne à analyser.
 * @return int 1 si la ligne est une déclaration de couleur, 0 sinon.
 *
 */
int is_color_line(char *line)
{
	if (!line || ft_strlen(line) < 2)
		return (0);
	if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0)
		return (1);
	return (0);
}

/**
 * @brief Détecte si une ligne fait partie de la map.
 *
 * Vérifie que la ligne ne contient que des caractères valides pour une
 * ligne de carte : '0', '1', ' ', 'N', 'S', 'E', 'W', '\n'. Tout autre
 * caractère invalide la ligne comme ligne de map.
 *
 * Utilisé pour identifier le début de la section map après les en-têtes.
 *
 * @param line Chaîne à analyser.
 * @return int 1 si la ligne contient uniquement des caractères de map, 0 sinon.
 *
 */
int is_map_line(char *line)
{
	int i;

	if (!line)
		return (0);
	i = 0;
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != ' ' && line[i] != 'N' && line[i] != 'S' && line[i] != 'E' && line[i] != 'W' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Compte le nombre de lignes dans un fichier.
 *
 * Ouvre le fichier spécifié et compte toutes ses lignes en utilisant
 * get_next_line(). Libère proprement chaque ligne lue et ferme le
 * descripteur de fichier. Appelle get_next_line(-1) pour nettoyer
 * les buffers internes.
 *
 * @param filename Chemin vers le fichier à analyser.
 * @return int Nombre de lignes dans le fichier, ou -1 si erreur d'ouverture.
 *
 */
int count_lines(const char *filename)
{
	int fd;
	char *line;
	int count;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	count = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	get_next_line(-1);
	return (count);
}
