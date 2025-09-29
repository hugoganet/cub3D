#include "cub3d.h"
#include <mlx.h>

int	load_single_texture(t_app *app, char *path, t_img *texture)
{
	int	width;
	int	height;

	texture->ptr = NULL;
	texture->addr = NULL;
	texture->w = 0;
	texture->h = 0;
	texture->ptr = mlx_xpm_file_to_image(app->mlx, path, &width, &height);
	if (!texture->ptr)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd("Failed to load texture: ", 2);
		ft_putendl_fd(path, 2);
		return (1);
	}
	texture->addr = mlx_get_data_addr(texture->ptr, &texture->bpp,
			&texture->line_len, &texture->endian);
	if (!texture->addr)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd("Failed to get texture data: ", 2);
		ft_putendl_fd(path, 2);
		mlx_destroy_image(app->mlx, texture->ptr);
		texture->ptr = NULL;
		return (1);
	}
	texture->w = width;
	texture->h = height;
	return (0);
}

static void	cleanup_north_texture(t_app *app)
{
	if (app->tex.north.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.north.ptr);
		app->tex.north.ptr = NULL;
	}
}

static void	cleanup_north_south_textures(t_app *app)
{
	cleanup_north_texture(app);
	if (app->tex.south.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.south.ptr);
		app->tex.south.ptr = NULL;
	}
}

static void	cleanup_north_south_west_textures(t_app *app)
{
	cleanup_north_south_textures(app);
	if (app->tex.west.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.west.ptr);
		app->tex.west.ptr = NULL;
	}
}

int	load_textures(t_app *app)
{
	if (load_single_texture(app, app->tex.north_path, &app->tex.north))
		return (1);
	if (load_single_texture(app, app->tex.south_path, &app->tex.south))
	{
		cleanup_north_texture(app);
		return (1);
	}
	if (load_single_texture(app, app->tex.west_path, &app->tex.west))
	{
		cleanup_north_south_textures(app);
		return (1);
	}
	if (load_single_texture(app, app->tex.east_path, &app->tex.east))
	{
		cleanup_north_south_west_textures(app);
		return (1);
	}
	app->tex.loaded = true;
	return (0);
}

static void	free_texture_paths(t_app *app)
{
	if (app->tex.north_path)
	{
		free(app->tex.north_path);
		app->tex.north_path = NULL;
	}
	if (app->tex.south_path)
	{
		free(app->tex.south_path);
		app->tex.south_path = NULL;
	}
	if (app->tex.east_path)
	{
		free(app->tex.east_path);
		app->tex.east_path = NULL;
	}
	if (app->tex.west_path)
	{
		free(app->tex.west_path);
		app->tex.west_path = NULL;
	}
}

void	free_textures(t_app *app)
{
	if (app->tex.north.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.north.ptr);
		app->tex.north.ptr = NULL;
	}
	if (app->tex.south.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.south.ptr);
		app->tex.south.ptr = NULL;
	}
	if (app->tex.west.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.west.ptr);
		app->tex.west.ptr = NULL;
	}
	if (app->tex.east.ptr)
	{
		mlx_destroy_image(app->mlx, app->tex.east.ptr);
		app->tex.east.ptr = NULL;
	}
	free_texture_paths(app);
	app->tex.loaded = false;
}

int	get_texture_pixel(t_img *texture, int x, int y)
{
	char	*pixel;
	int		color;

	if (x < 0 || x >= texture->w || y < 0 || y >= texture->h)
		return (0x000000);
	pixel = texture->addr + (y * texture->line_len + x * (texture->bpp / 8));
	color = *(int *)pixel;
	return (color & 0xFFFFFF);
}

t_img	*get_wall_texture(t_app *app, int side, t_vec2 ray_dir)
{
	if (side == 0)
	{
		if (ray_dir.x > 0)
			return (&app->tex.east);
		else
			return (&app->tex.west);
	}
	else
	{
		if (ray_dir.y > 0)
			return (&app->tex.south);
		else
			return (&app->tex.north);
	}
}
