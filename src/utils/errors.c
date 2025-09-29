#include "cub3d.h"
#include "libft.h"
#include <stdlib.h>

void	error_exit(t_app *app, const char *msg)
{
	ft_putendl_fd("Error", 2);
	if (msg)
		ft_putendl_fd((char *)msg, 2);
	app_destroy(app, 1);
	exit(1);
}

int	rgb_to_int(t_color c)
{
	if (c.r < 0)
		c.r = 0;
	if (c.r > 255)
		c.r = 255;
	if (c.g < 0)
		c.g = 0;
	if (c.g > 255)
		c.g = 255;
	if (c.b < 0)
		c.b = 0;
	if (c.b > 255)
		c.b = 255;
	return ((0xFF << 24) | (c.r << 16) | (c.g << 8) | c.b);
}
