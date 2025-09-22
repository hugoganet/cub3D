#include "cub3d.h"

// Fonction appelée quand une touche est pressée
int key_press(int keycode, t_app *app)
{
	printf("🔵 Key pressed: %d\n", keycode);  // Debug pour voir les codes

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

// Fonction appelée quand une touche est relâchée
int key_release(int keycode, t_app *app)
{
	printf("🔴 Key released: %d\n", keycode);  // Debug

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
