#include "cub3d.h"
#include "libft.h"
#include <mlx.h>

static void init_defaults(t_app *app)
{
	app->win_w = 1024;
	app->win_h = 768;
	app->move_speed = 0.06;
	app->rot_speed = 0.04;
	app->floor = (t_color){50, 50, 50};
	app->ceil = (t_color){135, 206, 235};
	app->keys = (t_keys){0};
}

int main(int argc, char **argv)
{
	t_app app;

	if (argc != 2)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("Usage: ./cub3D <map.cub>", 2);
		return (1);
	}

	init_defaults(&app);
	if (app_init(&app, app.win_w, app.win_h) != 0)
		return (1);
	if (parse_cub_file(&app, argv[1]) != 0)
	{
		app_destroy(&app, 1);
		return (1);
	}

	mlx_loop_hook(app.mlx, (int (*)(void *))app_loop, &app);
	mlx_hook(app.win, 2, 1L << 0, key_press, &app);
	mlx_hook(app.win, 3, 1L << 1, key_release, &app);
	mlx_hook(app.win, 17, 0, close_window, &app);
	mlx_loop(app.mlx);
	app_destroy(&app, 0);
	return (0);
}
