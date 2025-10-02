/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:35:37 by ncrivell          #+#    #+#             */
/*   Updated: 2025/10/02 13:06:49 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief Vérifie qu'une chaîne contient uniquement des chiffres
 * @param str Chaîne à vérifier
 * @return 1 si valide (que des chiffres), 0 sinon
 */
static int	is_valid_number(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Valide et convertit les 3 parties RGB en entiers
 * @param rgb_parts Tableau de 3 strings ["R", "G", "B"]
 * @param color Structure t_color à remplir
 * @return 0 si succès, 1 si erreur
 */
static int	validate_and_convert_rgb(char **rgb_parts, t_color *color)
{
	int	r;
	int	g;
	int	b;

	if (!is_valid_number(rgb_parts[0]) || !is_valid_number(rgb_parts[1])
		|| !is_valid_number(rgb_parts[2]))
		return (error_msg("RGB values must be numeric (0-255)"));
	r = ft_atoi(rgb_parts[0]);
	g = ft_atoi(rgb_parts[1]);
	b = ft_atoi(rgb_parts[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (error_msg("RGB values must be in range 0-255"));
	color->r = r;
	color->g = g;
	color->b = b;
	return (0);
}

/**
 * @brief Parse les valeurs RGB depuis une chaîne "R,G,B"
 * @param rgb_str Chaîne contenant "R,G,B"
 * @param color Structure t_color à remplir
 * @return 0 si succès, 1 si erreur
 */
int	parse_rgb_values(const char *rgb_str, t_color *color)
{
	char	**rgb_parts;
	int		count;
	int		result;

	rgb_parts = ft_split(rgb_str, ',');
	if (!rgb_parts)
		return (error_msg("Memory allocation failed for RGB parsing"));
	count = 0;
	while (rgb_parts[count])
		count++;
	if (count != 3)
	{
		free_split(rgb_parts);
		return (error_msg("RGB format must be R,G,B (3 values)"));
	}
	result = validate_and_convert_rgb(rgb_parts, color);
	free_split(rgb_parts);
	return (result);
}

/**
 * @brief Extrait la partie RGB d'une ligne "F R,G,B" ou "C R,G,B"
 * @param line Ligne complète à parser
 * @note Ignorer le préfixe (F ou C) et les espaces
 * @note Trouver la fin (ignorer \n)
 * @note Allouer et copier
 * @return Chaîne "R,G,B" ou NULL si erreur
 */
char	*extract_rgb_string(char *line)
{
	char	*rgb_str;
	int		i;
	int		j;
	int		start;

	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	while (line[i] && line[i] == ' ')
		i++;
	start = i;
	while (line[i] && line[i] != '\n')
		i++;
	rgb_str = malloc(i - start + 1);
	if (!rgb_str)
		return (NULL);
	j = 0;
	while (start < i)
		rgb_str[j++] = line[start++];
	rgb_str[j] = '\0';
	return (rgb_str);
}

/**
 * @brief Parse une ligne de couleur complète (F ou C)
 * @param app Structure principale
 * @param line Ligne à parser ("F R,G,B" ou "C R,G,B")
 * @note Extraire la partie RGB de la ligne
 * @note Parser les valeurs RGB
 * @note Déterminer si c'est Floor ou Ceiling
 * @return 0 si succès, 1 si erreur
 */
int	parse_color_line(t_app *app, char *line)
{
	char	*rgb_str;
	t_color	color;

	rgb_str = extract_rgb_string(line);
	if (!rgb_str)
		return (error_msg("Memory allocation failed for color parsing"));
	if (parse_rgb_values(rgb_str, &color) != 0)
	{
		free(rgb_str);
		return (-1);
	}
	if (ft_strncmp(line, "F ", 2) == 0)
		app->floor = color;
	else if (ft_strncmp(line, "C ", 2) == 0)
		app->ceil = color;
	else
	{
		free(rgb_str);
		return (error_msg("Color line must start with F or C"));
	}
	free(rgb_str);
	return (0);
}
