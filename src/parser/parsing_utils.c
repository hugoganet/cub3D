#include "cub3d.h"
#include "libft.h"

int set_defaults_after_parse(t_app *app)
{
	// Temporary map defaults until full parser is implemented
	app->map.grid = NULL;
	app->map.width = 0;
	app->map.height = 0;
	return (0);
}

int is_texture_line(char *line)
{
	if (!line || ft_strlen(line) < 3)
		return (0);

	if (ft_strncmp(line, "NO ", 3) == 0 ||
		ft_strncmp(line, "SO ", 3) == 0 ||
		ft_strncmp(line, "WE ", 3) == 0 ||
		ft_strncmp(line, "EA ", 3) == 0)
		return (1);
	return (0);
}

int is_color_line(char *line)
{
	if (!line || ft_strlen(line) < 2)
		return (0);

	if (ft_strncmp(line, "F ", 2) == 0 ||
		ft_strncmp(line, "C ", 2) == 0)
		return (1);
	return (0);
}

int is_map_line(char *line)
{
	int i;

	if (!line)
		return (0);

	i = 0;
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != ' ' &&
			line[i] != 'N' && line[i] != 'S' && line[i] != 'E' &&
			line[i] != 'W' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int count_lines(const char *filename)
{
	int fd;
	char *line;
	int count;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);

	count = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		count++;
		free(line);
	}

	close(fd);
	return (count);
}
