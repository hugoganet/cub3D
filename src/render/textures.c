#include "cub3d.h"
#include <mlx.h>

int load_textures(t_app *app)
{
	(void)app;
	// Will load XPMs from parser-provided paths later
	return (0);
}

void free_textures(t_app *app)
{
	if (!app->mlx)
		return;
	if (app->tex.north.ptr)
		mlx_destroy_image(app->mlx, app->tex.north.ptr);
	if (app->tex.south.ptr)
		mlx_destroy_image(app->mlx, app->tex.south.ptr);
	if (app->tex.west.ptr)
		mlx_destroy_image(app->mlx, app->tex.west.ptr);
	if (app->tex.east.ptr)
		mlx_destroy_image(app->mlx, app->tex.east.ptr);
}
