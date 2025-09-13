/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:10:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/09/10 11:09:19 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_map	*init_map(void)
{
	t_map	*map;

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
	map->elements_count = 0;
	map->textures.north = NULL;
	map->textures.south = NULL;
	map->textures.west = NULL;
	map->textures.east = NULL;
	map->colors.floor_r = -1;
	map->colors.floor_g = -1;
	map->colors.floor_b = -1;
	map->colors.ceiling_r = -1;
	map->colors.ceiling_b = -1;
	map->colors.ceiling_g = -1;
	return (map);
}

int	is_valid_element(char *line)
{
	if ((ft_strncmp(line, "NO ", 3) == 0) || 
		(ft_strncmp(line, "SO ", 3) == 0) ||
		(ft_strncmp(line, "WE ", 3) == 0) || 
		(ft_strncmp(line, "EA ", 3) == 0) ||
		(ft_strncmp(line, "F ", 2) == 0) || 
		(ft_strncmp(line, "C ", 2) == 0))
		return (1);
	return (0);
}

int	validate_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != '1' && line[i] != '0' && line[i] != 'N' && 
			line[i] != 'S' && line[i] != 'E' && line[i] != 'W' && 
			line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int	process_line(char *line, t_map *map, int fd)
{
	if (ft_strncmp(line, "\n", ft_strlen(line)) == 0)
		return (0);
	if (map->elements_count < 6)
	{
		if (is_valid_element(line))
		{
			if (!load_texture(line, map))
			{
				free(line);
				free_textures(map);
				free(map);
				close(fd);
				return (ft_putstr_fd("Error\nInvalid texture or color format\n", 2), -1);
			}
			map->elements_count++;
		}
		else if (line[0] != ' ' && line[0] != '\t')
		{
			free(map);
			free_textures(map);
			close(fd);
			return (ft_putstr_fd("Error\nInvalid element in configuration\n", 2), -1);
		}
	}
	else
	{
		if (!validate_map_line(line))
		{
			free(line);
			free_textures(map);
			free(map);
			close(fd);
			return (ft_putstr_fd("Error\nInvalid character in map\n", 2), -1);
		}
		map->height++;
	}
	return (0);
}

int	validate_final_map(t_map *map)
{
	if (map->elements_count != 6)
	{
		free_textures(map);
		free(map);
		return (ft_putstr_fd("Error\nMissing elements (need NO, SO, WE, EA, F, C)\n", 2), -1);
	}
	if (map->height == 0)
	{
		free_textures(map);
		free(map);
		return (ft_putstr_fd("Error\nNo map found\n", 2), -1);
	}
	
	return (0);
}
