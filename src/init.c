#include "cub3d.h"
#include "libft.h"
#include <mlx.h>
#include <stdlib.h>

static int	create_frame(t_app *app, int w, int h)
{
	app->frame.ptr = mlx_new_image(app->mlx, w, h);
	if (!app->frame.ptr)
		return (-1);
	app->frame.addr = mlx_get_data_addr(app->frame.ptr, &app->frame.bpp,
			&app->frame.line_len, &app->frame.endian);
	app->frame.w = w;
	app->frame.h = h;
	return (0);
}

int	app_init(t_app *app, int w, int h)
{
	app->win_w = w;
	app->win_h = h;
	app->mlx = mlx_init();
	if (!app->mlx)
		return (ft_putendl_fd("Error\nmlx_init failed", 2), -1);
	if (load_textures(app) != 0)
		return (1);
	app->win = mlx_new_window(app->mlx, w, h, "cub3D");
	if (!app->win)
		return (ft_putendl_fd("Error\nmlx_new_window failed", 2), -1);
	if (create_frame(app, w, h) != 0)
		return (ft_putendl_fd("Error\nmlx_new_image failed", 2), -1);
	return (0);
}

void	free_map(t_app *app)
{
	int	i;

	if (!app->map.grid)
		return ;
	i = 0;
	while (i < app->map.height)
	{
		free(app->map.grid[i]);
		i++;
	}
	free(app->map.grid);
	app->map.grid = NULL;
	app->map.height = 0;
	app->map.width = 0;
}

void	app_destroy(t_app *app, int code)
{
	free_textures(app);
	gnl_free(NULL);
	if (app->frame.ptr)
		mlx_destroy_image(app->mlx, app->frame.ptr);
	if (app->win)
		mlx_destroy_window(app->mlx, app->win);
	if (app->mlx)
	{
		mlx_destroy_display(app->mlx);
		free(app->mlx);
		app->mlx = NULL;
	}
	free_map(app);
	(void)code;
}
