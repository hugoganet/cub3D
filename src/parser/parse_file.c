#include "cub3d.h"
#include "libft.h"

static int set_defaults_after_parse(t_app *app)
{
	// Temporary map defaults until full parser is implemented
	app->map.grid = NULL;
	app->map.width = 0;
	app->map.height = 0;
	return (0);
}

int parse_cub_file(t_app *app, const char *path)
{
	(void)path;
	// TODO: implement full .cub parsing
	if (load_textures(app) != 0)
		return (ft_putendl_fd("Error\nFailed to load textures", 2), -1);
	return (set_defaults_after_parse(app));
}
