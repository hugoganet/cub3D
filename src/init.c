#include "cub3d.h"
#include "libft.h"
#include <mlx.h>
#include <stdlib.h>

static int create_frame(t_app *app, int w, int h)
{
	app->frame.ptr = mlx_new_image(app->mlx, w, h);
	if (!app->frame.ptr)
		return (-1);
	app->frame.addr = mlx_get_data_addr(app->frame.ptr, &app->frame.bpp, &app->frame.line_len, &app->frame.endian);
	app->frame.w = w;
	app->frame.h = h;
	return (0);
}

int app_init(t_app *app, int w, int h)
{
	ft_bzero(app, sizeof(*app));
	app->mlx = mlx_init();
	if (!app->mlx)
		return (ft_putendl_fd("Error\nmlx_init failed", 2), -1);
	app->win = mlx_new_window(app->mlx, w, h, "cub3D");
	if (!app->win)
		return (ft_putendl_fd("Error\nmlx_new_window failed", 2), -1);
	if (create_frame(app, w, h) != 0)
		return (ft_putendl_fd("Error\nmlx_new_image failed", 2), -1);

	// Default player (will be overridden by parser)
	app->player.pos = (t_vec2){2.5, 2.5};
	app->player.dir = (t_vec2){-1.0, 0.0};
	app->player.plane = (t_vec2){0.0, 0.66};
	return (0);
}

void app_destroy(t_app *app, int code)
{
	free_textures(app);
	if (app->frame.ptr)
		mlx_destroy_image(app->mlx, app->frame.ptr);
	if (app->win)
		mlx_destroy_window(app->mlx, app->win);
	// On macOS, mlx pointer is managed by the framework; no mlx_destroy_display
	(void)code;
}
