#include "cub3d.h"
#include <mlx.h>

static void clear_frame(t_img *img, int color)
{
	int x;
	int y;
	for (y = 0; y < img->h; ++y)
	{
		for (x = 0; x < img->w; ++x)
			img_put_pixel(img, x, y, color);
	}
}



void render_frame(t_app *app)
{
	/* Simple sky/floor background for now */
	clear_frame(&app->frame, rgb_to_int(app->ceil));
	int floor_color = rgb_to_int(app->floor);
	int y;
	int x;
	for (y = app->frame.h / 2; y < app->frame.h; ++y)
	{
		for (x = 0; x < app->frame.w; ++x)
			img_put_pixel(&app->frame, x, y, floor_color);
	}
	/* Raycasting to be added here */
}




// Mettre un pixel dans l'image
void img_put_pixel(t_img *img, int x, int y, int color)
{
    char *dst;

    if (x < 0 || y < 0 || x >= 1024 || y >= 768)  // Bounds check
        return;

    dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int*)dst = color;
}

// Remplir l'arrière-plan
void fill_background(t_app *app, int color)
{
    int x, y;

    y = 0;
    while (y < app->win_h)
    {
        x = 0;
        while (x < app->win_w)
        {
            img_put_pixel(&app->frame, x, y, color);
            x++;
        }
        y++;
    }
}

// Dessiner un rectangle
void draw_rect(t_app *app, int x, int y, int w, int h, int color)
{
    int i, j;

    j = 0;
    while (j < h)
    {
        i = 0;
        while (i < w)
        {
            img_put_pixel(&app->frame, x + i, y + j, color);
            i++;
        }
        j++;
    }
}
