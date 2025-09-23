#include "cub3d.h"

double calculate_wall_height(double perp_dist, int screen_h)
{
    // Avoid division by zero
    if (perp_dist < 0.001)
        perp_dist = 0.001;

    // Calculate wall height on screen
    return (screen_h / perp_dist);
}

void calculate_wall_bounds(int height, int screen_h, int *draw_start, int *draw_end)
{
    // Calculate top of wall strip
    *draw_start = -height / 2 + screen_h / 2;
    if (*draw_start < 0)
        *draw_start = 0;

    // Calculate bottom of wall strip
    *draw_end = height / 2 + screen_h / 2;
    if (*draw_end >= screen_h)
        *draw_end = screen_h - 1;
}

void draw_wall_column(t_app *app, int x, int draw_start, int draw_end, int color)
{
    int y;

    y = draw_start;
    while (y <= draw_end)
    {
        img_put_pixel(&app->frame, x, y, color);
        y++;
    }
}

int get_wall_color(int wall_face)
{
    // Different colors for each wall face (debug colors)
    if (wall_face == FACE_NORTH)
        return 0xFFFFFF;  // White - North
    else if (wall_face == FACE_SOUTH)
        return 0xAAAAAA;  // Light gray - South
    else if (wall_face == FACE_EAST)
        return 0x888888;  // Medium gray - East
    else
        return 0x555555;  // Dark gray - West
}