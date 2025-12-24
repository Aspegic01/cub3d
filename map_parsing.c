/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:10:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/11/17 18:05:06 by mlabrirh         ###   ########.fr       */
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
	map->map_line_index = 0;
	map->line_count = 0;
	map->player_count = 0;
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

int	free_tex_map(t_map *map, int fd, char *error)
{
	return (free_textures(map), free(map), close(fd)
		, ft_putstr_fd(error, 2), -1);
}

static int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ' || line[i] == '\t')
		i++;
	if (ft_strncmp(line, "\n", ft_strlen(line)) == 0)
		return (0);
	return (1);
}

int	process_line(char *line, t_map *map, int fd)
{

	if (!is_empty_line(line))
		return (0);
	if (map->elements_count < 6)
	{
		if (is_valid_element(line))
		{
			if (!load_texture(line, map))
				return (free_tex_map(map, fd
						, "Error\nInvalid texture or color format\n"));
			map->elements_count++;
		}
		else if (line[0] != ' ' && line[0] != '\t')
			return (free_tex_map(map, fd
					, "Error\nInvalid element in configuration\n"));
	}
	else
	{
		if (!validate_map_line(line))
			return (free_tex_map(map, fd
					, "Error\nInvalid character in map\n"));
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

void	print_map(t_map *map)
{
	int i;

	if (!map)
	{
		ft_putstr_fd("(null map)\n", 1);
		return;
	}
	if (!map->grid)
	{
		ft_putstr_fd("(empty grid)\n", 1);
		return;
	}
	i = 0;
	while (i < map->height && map->grid[i])
	{
		ft_putstr_fd(map->grid[i], 1);
		/* ensure newline at end of row for readability */
		if (map->grid[i][ft_strlen(map->grid[i]) - 1] != '\n')
			ft_putstr_fd("\n", 1);
		i++;
	}
}
