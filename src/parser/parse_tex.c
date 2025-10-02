/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncrivell <ncrivell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:18:41 by ncrivell          #+#    #+#             */
/*   Updated: 2025/09/29 13:22:29 by ncrivell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

/**
 * @brief Extrait le chemin de fichier depuis une ligne de configuration.
 *
 * Parcourt la ligne à partir du premier token (ex: "NO", "SO", ...) puis
 * saute les espaces et copie la séquence de caractères jusqu'au prochain
 * espace ou '\n' dans une nouvelle allocation.
 *
 * @param line Ligne d'entrée (ex: "NO ./path/to/texture.xpm\n").
 * @return char* Chaîne allouée contenant le chemin (NULL si allocation échoue).
 *               L'appelant est responsable de free().
 */
char	*extract_path(char *line)
{
	char	*path;
	int		i;
	int		j;
	int		start;

	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	while (line[i] && line[i] == ' ')
		i++;
	start = i;
	while (line[i] && line[i] != '\n' && line[i] != ' ')
		i++;
	path = malloc(i - start + 1);
	if (!path)
		return (NULL);
	j = 0;
	while (start < i)
		path[j++] = line[start++];
	path[j] = '\0';
	return (path);
}

/**
 * @brief Assigne un chemin de texture à une destination si non déjà définie.
 *
 * Vérifie si la destination (*dest) est NULL. Si elle est déjà définie,
 * retourne -1 (erreur de duplication). Sinon, duplique le chemin via
 * ft_strdup() et l'assigne à *dest.
 *
 * @param dest Pointeur vers la variable de destination (ex: &app->tex.north_path).
 * @param path Chemin à assigner.
 * @return int 0 si succès, -1 si *dest était déjà défini (duplication).
 *
 * @see assign_texture() qui utilise cette fonction pour chaque direction
 */
static int	set_texture_path(char **dest, char *path)
{
	if (*dest)
		return (-1);
	*dest = ft_strdup(path);
	return (0);
}

/**
 * @brief Assigne un chemin de texture selon le préfixe de la ligne.
 *
 * Identifie la direction de texture (NO/SO/WE/EA) en analysant les 3 premiers
 * caractères de la ligne, puis délègue à set_texture_path() pour assigner
 * le chemin à la variable appropriée dans app->tex.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @param line Ligne de texture (ex: "NO ./textures/north.xpm").
 * @param path Chemin extrait de la ligne.
 * @return int 0 si succès, -1 si duplication détectée.
 *
 * @see set_texture_path() pour l'assignation avec vérification de duplication
 * @see parse_texture_line() qui appelle cette fonction
 */
static int	assign_texture(t_app *app, char *line, char *path)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (set_texture_path(&app->tex.north_path, path));
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (set_texture_path(&app->tex.south_path, path));
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (set_texture_path(&app->tex.west_path, path));
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (set_texture_path(&app->tex.east_path, path));
	return (0);
}

/**
 * @brief Parse une ligne de définition de texture complète.
 *
 * Extrait le chemin de texture depuis la ligne (via extract_path), puis
 * assigne ce chemin à la variable appropriée dans app->tex selon le préfixe
 * de la ligne (NO/SO/WE/EA). Libère le chemin extrait avant de retourner.
 *
 * En cas d'erreur d'allocation mémoire, appelle error_exit().
 * En cas de duplication de texture, retourne -1.
 *
 * @param app Pointeur vers la structure principale de l'application.
 * @param line Ligne complète à parser (ex: "NO ./textures/north_wall.xpm").
 * @return int 0 si succès, -1 si erreur (duplication de texture).
 *
 * @see extract_path() pour l'extraction du chemin depuis la ligne
 * @see assign_texture() pour l'assignation à la bonne variable
 */
int	parse_texture_line(t_app *app, char *line)
{
	char	*path;
	int		result;

	path = extract_path(line);
	if (!path)
		error_exit(app, "Memory allocation failed");
	result = assign_texture(app, line, path);
	free(path);
	if (result != 0)
		return (-1);
	return (0);
}
