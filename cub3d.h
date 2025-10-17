/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:38 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/10/17 11:23:23 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>
# include "./get_next_line/get_next_line.h"
# include "./libft/libft.h"
# include "./minilibx-linux/mlx.h"

# define WIN_WIDTH 1024
# define WIN_HEIGHT 768
# define WIN_TITLE "Cub3D"

typedef struct s_player
{
    double  x;
    double  y;
    double  dir_x;
    double  dir_y;
    double  plane_x;
    double  plane_y;
}               t_player;

typedef struct s_textures
{
    char    *north;
    char    *south;
    char    *west;
    char    *east;
}               t_textures;

typedef struct s_colors
{
    int     floor_r;
    int     floor_g;
    int     floor_b;
    int     ceiling_r;
    int     ceiling_g;
    int     ceiling_b;
}               t_colors;

typedef struct s_map
{
    char        **grid;
    int         width;
    int         height;
    int         elements_count;
    t_textures  textures;
    t_colors    colors;
}               t_map;

typedef struct s_game
{
    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
    t_map   *map;
}               t_game;

int     init_game(char *map_file);
t_map   *read_map(char *map_file);

// Map parsing functions
t_map   *init_map(void);
int     is_valid_element(char *line);
int     validate_map_line(char *line);
int     process_line(char *line, t_map *map, int fd);
int     validate_final_map(t_map *map);
int     allocate_map_grid(t_map *map);
int     store_map_line(char *line, t_map *map, int line_index);
int     load_map_data(char *map_file, t_map *map);
int     is_valid_map_char(char c);
int     check_first_last_row(t_map *map, int row);
int     check_side_walls(t_map *map);
int     is_valid_neighbor(t_map *map, int row, int col);
int     validate_walkable_spaces(t_map *map);
int     check_map_surrounded_by_walls(t_map *map);

// Texture and color loading functions
char    *extract_path(char *line);
int     parse_color(char *line, int *r, int *g, int *b);
int     load_texture(char *line, t_map *map);

// Player initialization functions
void    set_player_direction(t_player *player, char direction);
int     find_player_position(t_map *map, t_player *player);
int     init_player(t_map *map, t_player *player);

// Memory management functions
void    free_textures(t_map *map);
void    free_map_grid(t_map *map);
void    free_game(void);
void    free_map(void);
void    free_player(void);

// Map utility functions
void    fix_zero_space_to_zero(t_map *map);

// Window and game functions
int     init_window(t_game *game);
int     close_window(t_game *game);
int     handle_keypress(int keycode, t_game *game);
void    render_frame(t_game *game);
int     game_loop(t_game *game);



#endif