#include "cub3d.h"
#include "libft.h"
#include <mlx.h>



int main(int argc, char **argv)
{
	t_app app;

	if (parsing(&app, argc, argv) != 0)
		return 1;
	// Initialiser MLX après parsing réussi
	if (app_init(&app, app.win_w, app.win_h) != 0)
	{
		// Fixed: cleanup parsed data when texture loading fails
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
