#include "cub3d.h"
#include <stdlib.h>

#define KEY_ESC 65307
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_LEFT 65361
#define KEY_RIGHT 65363

int key_press(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		close_window(app);
	else if (keycode == KEY_W)
		app->keys.w = true;
	else if (keycode == KEY_A)
		app->keys.a = true;
	else if (keycode == KEY_S)
		app->keys.s = true;
	else if (keycode == KEY_D)
		app->keys.d = true;
	else if (keycode == KEY_LEFT)
		app->keys.left = true;
	else if (keycode == KEY_RIGHT)
		app->keys.right = true;
	return (0);
}

int key_release(int keycode, t_app *app)
{
	if (keycode == KEY_W)
		app->keys.w = false;
	else if (keycode == KEY_A)
		app->keys.a = false;
	else if (keycode == KEY_S)
		app->keys.s = false;
	else if (keycode == KEY_D)
		app->keys.d = false;
	else if (keycode == KEY_LEFT)
		app->keys.left = false;
	else if (keycode == KEY_RIGHT)
		app->keys.right = false;
	return (0);
}
