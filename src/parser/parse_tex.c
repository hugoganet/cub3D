#include "cub3d.h"
#include "libft.h"

static char *extract_path(char *line)
{
	char *path;
	int i, j, start;

	// Ignorer le prefixe (NO, SO, WE, EA) et les espaces
	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	while (line[i] && line[i] == ' ')
		i++;

	// Trouver le début du chemin
	start = i;

	// Trouver la fin (ignorer \n et espaces)
	while (line[i] && line[i] != '\n' && line[i] != ' ')
		i++;

	// Allouer et copier le chemin
	path = malloc(i - start + 1);
	if (!path)
		return (NULL);

	j = 0;
	while (start < i)
		path[j++] = line[start++];
	path[j] = '\0';

	return (path);
}

int parse_texture_line(t_app *app, char *line)
{
	char *path;

	path = extract_path(line);
	if (!path)
		error_exit(app, "Memory allocation failed");

	// Stocker selon le prefixe
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		app->tex.north_path = ft_strdup(path);
	}
	else if (ft_strncmp(line, "SO ", 3) == 0)
	{
		app->tex.south_path = ft_strdup(path);
	}
	else if (ft_strncmp(line, "WE ", 3) == 0)
	{
		app->tex.west_path = ft_strdup(path);
	}
	else if (ft_strncmp(line, "EA ", 3) == 0)
	{
		app->tex.east_path = ft_strdup(path);
	}
	free(path);
	return (0);
}

