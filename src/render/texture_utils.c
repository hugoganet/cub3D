/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:45:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/09/29 14:45:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <mlx.h>

static void	print_texture_error(char *path, char *message)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(message, 2);
	ft_putendl_fd(path, 2);
}

static int	load_texture_image(t_app *app, char *path, t_img *texture)
{
	int	width;
	int	height;

	texture->ptr = mlx_xpm_file_to_image(app->mlx, path, &width, &height);
	if (!texture->ptr)
	{
		print_texture_error(path, "Failed to load texture: ");
		return (1);
	}
	texture->w = width;
	texture->h = height;
	return (0);
}

static int	get_texture_data(t_app *app, char *path, t_img *texture)
{
	texture->addr = mlx_get_data_addr(texture->ptr, &texture->bpp,
			&texture->line_len, &texture->endian);
	if (!texture->addr)
	{
		print_texture_error(path, "Failed to get texture data: ");
		mlx_destroy_image(app->mlx, texture->ptr);
		texture->ptr = NULL;
		return (1);
	}
	return (0);
}

int	load_single_texture(t_app *app, char *path, t_img *texture)
{
	texture->ptr = NULL;
	texture->addr = NULL;
	texture->w = 0;
	texture->h = 0;
	if (load_texture_image(app, path, texture))
		return (1);
	if (get_texture_data(app, path, texture))
		return (1);
	return (0);
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
