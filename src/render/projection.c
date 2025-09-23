#include "cub3d.h"
#include <math.h>

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

int get_texture_coord_x(double wall_x, t_img *texture)
{
    int tex_x = (int)(wall_x * (double)texture->w);
    if (tex_x < 0)
        tex_x = 0;
    if (tex_x >= texture->w)
        tex_x = texture->w - 1;
    return tex_x;
}

void draw_textured_wall_column(t_app *app, int x, int draw_start, int draw_end, t_ray_hit *hit)
{
    // Get appropriate texture based on wall face
    t_img *texture;
    if (hit->wall_face == FACE_NORTH)
        texture = &app->tex.north;
    else if (hit->wall_face == FACE_SOUTH)
        texture = &app->tex.south;
    else if (hit->wall_face == FACE_EAST)
        texture = &app->tex.east;
    else
        texture = &app->tex.west;

    // Calculate texture X coordinate
    int tex_x = get_texture_coord_x(hit->wall_x, texture);

    // Calculate how much to increase tex_y per pixel
    int wall_height = draw_end - draw_start;
    double step = (double)texture->h / (double)wall_height;
    double tex_pos = (draw_start - app->win_h / 2 + wall_height / 2) * step;

    // Draw textured column
    int y = draw_start;
    while (y <= draw_end)
    {
        int tex_y = (int)tex_pos;
        if (tex_y >= texture->h)
            tex_y = texture->h - 1;
        if (tex_y < 0)
            tex_y = 0;

        int color = get_texture_pixel(texture, tex_x, tex_y);
        img_put_pixel(&app->frame, x, y, color);

        tex_pos += step;
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