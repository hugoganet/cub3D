/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hugoganet <hugoganet@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 00:00:00 by hugoganet         #+#    #+#             */
/*   Updated: 2025/10/01 00:00:01 by hugoganet        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"

int	handle_texture_line(t_app *app, char *line, t_parse_counters *counters)
{
	if (counters->texture_count == 4)
	{
		printf("Error\nOnly 4 texture path are allowed\n");
		return (-1);
	}
	if (parse_texture_line(app, line) != 0)
	{
		printf("Error\nDuplicate texture definition\n");
		return (-1);
	}
	counters->texture_count++;
	return (0);
}

int	handle_map_line(t_app *app, char *line, t_parse_counters *counters)
{
	if (!counters->map_started)
	{
		counters->map_started = 1;
		init_map(app);
	}
	add_map_line(app, line, counters->map_line_index);
	counters->map_line_index++;
	return (0);
}
