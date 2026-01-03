/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 20:59:23 by mlabrirh          #+#    #+#             */
/*   Updated: 2026/01/03 16:07:43 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*extract_path(char *line)
{
	int		start;
	int		end;
	int		len;
	char	*path;

	start = 0;
	while (line[start] && (line[start] == ' ' || line[start] == '\t'))
		start++;
	while (line[start] && line[start] != ' ' && line[start] != '\t')
		start++;
	while (line[start] && (line[start] == ' ' || line[start] == '\t'))
		start++;
	end = start;
	while (line[end] && line[end] != '\n'
		&& line[end] != ' ' && line[end] != '\t')
		end++;
	len = end - start;
	path = malloc(sizeof(char) * (len + 1));
	if (!path)
		return (NULL);
	ft_strlcpy(path, line + start, len + 1);
	return (path);
}

void	free_textures(t_map *map)
{
	if (map->textures.north)
		free(map->textures.north);
	if (map->textures.south)
		free(map->textures.south);
	if (map->textures.west)
		free(map->textures.west);
	if (map->textures.east)
		free(map->textures.east);
	if (map->tex_north)
		mlx_delete_texture(map->tex_north);
	if (map->tex_south)
		mlx_delete_texture(map->tex_south);
	if (map->tex_west)
		mlx_delete_texture(map->tex_west);
	if (map->tex_east)
		mlx_delete_texture(map->tex_east);
}
