/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:04:59 by klaayoun          #+#    #+#             */
/*   Updated: 2026/01/03 15:54:47 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	make_gun_path(char *buffer, int i)
{
	char	*prefix;
	int		j;
	char	*suffix;
	int		k;

	prefix = "textures/shitgun_animation/shitgun_animation_";
	j = 0;
	while (prefix[j])
	{
		buffer[j] = prefix[j];
		j++;
	}
	if (i < 10)
		buffer[j++] = i + '0';
	else
	{
		buffer[j++] = (i / 10) + '0';
		buffer[j++] = (i % 10) + '0';
	}
	suffix = ".png";
	k = 0;
	while (suffix[k])
		buffer[j++] = suffix[k++];
	buffer[j] = '\0';
}

void	free_gun_frames(t_game *game, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (game->gun.frames[i])
			mlx_delete_image(game->mlx, game->gun.frames[i]);
		i++;
	}
}
