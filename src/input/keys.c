#include "cub3d.h"
#include <stdlib.h>

/* macOS keycodes with MiniLibX */
#define KEY_ESC 53
#define KEY_W 13
#define KEY_A 0
#define KEY_S 1
#define KEY_D 2
#define KEY_LEFT 123
#define KEY_RIGHT 124

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
