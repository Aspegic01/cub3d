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
	if (ft_strncmp(line, "NO ", 3) == 0)
		map->textures.north = extract_path(line);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		map->textures.south = extract_path(line);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		map->textures.west = extract_path(line);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		map->textures.east = extract_path(line);
	else if (ft_strncmp(line, "F ", 2) == 0)
		return (parse_color(line, &map->colors.floor_r, &map->colors.floor_g, &map->colors.floor_b));
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (parse_color(line, &map->colors.ceiling_r, &map->colors.ceiling_g, &map->colors.ceiling_b));
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
	if (map->tex_north)
		mlx_delete_texture(map->tex_north);
	if (map->tex_south)
		mlx_delete_texture(map->tex_south);
	if (map->tex_west)
		mlx_delete_texture(map->tex_west);
	if (map->tex_east)
		mlx_delete_texture(map->tex_east);
}
