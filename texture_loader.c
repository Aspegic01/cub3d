/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_loader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 13:10:00 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/29 09:32:27 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft/libft.h"

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
	while (line[end] && line[end] != '\n' && line[end] != ' ' && line[end] != '\t')
		end++;
	len = end - start;
	if (len == 0)
		return (NULL);
	path = malloc(sizeof(char) * (len + 1));
	if (!path)
		return (NULL);
	ft_strlcpy(path, line + start, len + 1);
	return (path);
}

// bool ft_isnum(char *rgb)
// {
// 	int i = 0;
//
// 	if (!rgb || rgb[0] == '\0')
// 		return false; // empty string is not a number
//
// 	while (rgb[i])
// 	{
// 		if (!ft_isdigit((unsigned char)rgb[i]))
// 			return false; // non-digit character found
// 		i++;
// 	}
// 	return true; // all digits
// }

void	free_rgb(char **rgb)
{
	int	i;

	i = 0;
	while (rgb[i])
		free(rgb[i++]);
	free(rgb);
}

static int	is_valid_rgb_token(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	if (!s[i] || !ft_isdigit((unsigned char)s[i]))
		return (0);
	while (s[i] && ft_isdigit((unsigned char)s[i]))
		i++;
	while (s[i] && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'))
		i++;
	return (s[i] == '\0');
}

static int	color_er(char **rgb)
{
	
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
	{
		if (rgb)
			free_rgb(rgb);
		return (0);
	}
	if (rgb[3])
	{
		free_rgb(rgb);
		return (0);
	}
	if (!is_valid_rgb_token(rgb[0]) || !is_valid_rgb_token(rgb[1])\
		|| !is_valid_rgb_token(rgb[2]))
	{
		free_rgb(rgb);
		return (0);
	}
	return 1;
}

int	parse_color(char *line, int *r, int *g, int *b)
{
	char	**rgb;
	int		i;

	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	rgb = ft_split(line + i, ',');
	if (!color_er(rgb))
		return 0;
	*r = ft_atoi(rgb[0]);
	*g = ft_atoi(rgb[1]);
	*b = ft_atoi(rgb[2]);
	free_rgb(rgb);
	if (*r < 0 || *r > 255 || *g < 0 || *g > 255 || *b < 0 || *b > 255)
		return (0);
	return (1);
}

int	load_texture(char *line, t_map *map)
{
	char	*path;

	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		if (map->textures.north)
			return (0);
		path = extract_path(line);
		if (!path)
			return (0);
		map->textures.north = path;
	}
	else if (ft_strncmp(line, "SO ", 3) == 0)
	{
		if (map->textures.south)
			return (0);
		path = extract_path(line);
		if (!path)
			return (0);
		map->textures.south = path;
	}
	else if (ft_strncmp(line, "WE ", 3) == 0)
	{
		if (map->textures.west)
			return (0);
		path = extract_path(line);
		if (!path)
			return (0);
		map->textures.west = path;
	}
	else if (ft_strncmp(line, "EA ", 3) == 0)
	{
		if (map->textures.east)
			return (0);
		path = extract_path(line);
		if (!path)
			return (0);
		map->textures.east = path;
	}
	else if (ft_strncmp(line, "F ", 2) == 0)
	{
		if (map->colors.floor_r != -1)
			return (0);
		return (parse_color(line, &map->colors.floor_r, &map->colors.floor_g, &map->colors.floor_b));
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		if (map->colors.ceiling_r != -1)
			return (0);
		return (parse_color(line, &map->colors.ceiling_r, &map->colors.ceiling_g, &map->colors.ceiling_b));
	}
	return (1);
}

static int	check_texture_extension(char *path)
{
	int	len;

	len = ft_strlen(path);
	if (len < 5)
		return (0);
	if (ft_strncmp(path + len - 4, ".xpm", 4) == 0)
		return (1);
	if (len >= 5 && ft_strncmp(path + len - 4, ".png", 4) == 0)
		return (1);
	return (0);
}

int	validate_texture_files(t_map *map)
{
	int	fd;

	if (!check_texture_extension(map->textures.north))
		return (ft_putstr_fd("Error\nNorth texture must be .xpm or .png\n", 2), 0);
	if (!check_texture_extension(map->textures.south))
		return (ft_putstr_fd("Error\nSouth texture must be .xpm or .png\n", 2), 0);
	if (!check_texture_extension(map->textures.west))
		return (ft_putstr_fd("Error\nWest texture must be .xpm or .png\n", 2), 0);
	if (!check_texture_extension(map->textures.east))
		return (ft_putstr_fd("Error\nEast texture must be .xpm or .png\n", 2), 0);
	fd = open(map->textures.north, O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd("Error\nNorth texture file not found or unreadable\n", 2), 0);
	close(fd);
	fd = open(map->textures.south, O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd("Error\nSouth texture file not found or unreadable\n", 2), 0);
	close(fd);
	fd = open(map->textures.west, O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd("Error\nWest texture file not found or unreadable\n", 2), 0);
	close(fd);
	fd = open(map->textures.east, O_RDONLY);
	if (fd < 0)
		return (ft_putstr_fd("Error\nEast texture file not found or unreadable\n", 2), 0);
	close(fd);
	return (1);
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
}
