#include "cub3d.h"
#include <mlx.h>
#include <math.h>
#include <stdlib.h>

void apply_movement(t_app *app)
{
	double ms = app->move_speed;
	double rs = app->rot_speed;
	if (app->keys.left)
	{
		double oldDirX = app->player.dir.x;
		app->player.dir.x = app->player.dir.x * cos(-rs) - app->player.dir.y * sin(-rs);
		app->player.dir.y = oldDirX * sin(-rs) + app->player.dir.y * cos(-rs);
		double oldPlaneX = app->player.plane.x;
		app->player.plane.x = app->player.plane.x * cos(-rs) - app->player.plane.y * sin(-rs);
		app->player.plane.y = oldPlaneX * sin(-rs) + app->player.plane.y * cos(-rs);
	}
	if (app->keys.right)
	{
		double oldDirX = app->player.dir.x;
		app->player.dir.x = app->player.dir.x * cos(rs) - app->player.dir.y * sin(rs);
		app->player.dir.y = oldDirX * sin(rs) + app->player.dir.y * cos(rs);
		double oldPlaneX = app->player.plane.x;
		app->player.plane.x = app->player.plane.x * cos(rs) - app->player.plane.y * sin(rs);
		app->player.plane.y = oldPlaneX * sin(rs) + app->player.plane.y * cos(rs);
	}
	// Simple movement without collision yet (will be handled after parsing map)
	if (app->keys.w)
	{
		app->player.pos.x += app->player.dir.x * ms;
		app->player.pos.y += app->player.dir.y * ms;
	}
	if (app->keys.s)
	{
		app->player.pos.x -= app->player.dir.x * ms;
		app->player.pos.y -= app->player.dir.y * ms;
	}
	if (app->keys.a)
	{
		// strafe left
		app->player.pos.x += (-app->player.dir.y) * ms;
		app->player.pos.y += (app->player.dir.x) * ms;
	}
	if (app->keys.d)
	{
		// strafe right
		app->player.pos.x -= (-app->player.dir.y) * ms;
		app->player.pos.y -= (app->player.dir.x) * ms;
	}
}

int app_loop(t_app *app)
{
    // Fond noir
    fill_background(app, 0x000000);

    // Dessiner la minimap
    render_minimap(app);

    // Afficher le frame
    mlx_put_image_to_window(app->mlx, app->win, app->frame.ptr, 0, 0);

    return (0);
}

int close_window(t_app *app)
{
	app_destroy(app, 0);
	exit(0);
	return (0);
}
