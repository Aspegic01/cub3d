/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlabrirh <mlabrirh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:00:38 by mlabrirh          #+#    #+#             */
/*   Updated: 2025/12/26 17:08:10 by mlabrirh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "./get_next_line/get_next_line.h"
# include "./include/minilibx/include/MLX42/MLX42.h"
# include "./libft/libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define WIN_WIDTH 1280
# define WIN_HEIGHT 720
# define CELL_SIZE 12
# define CELL_COUNT 10
# define CELL_COUNT_HALF 5
# define PLAYER_SIZE 3
# define PLAYER_HALF 1
# define WIN_TITLE "Cub3D"
# define ROT_SPEED 3.0
# define MOVE_SPEED 0.025
# define MOUSE_SENSITIVITY 0.002
# define MAPFG 0xFFFFFFFF
# define MAPBG 0x333333FF

typedef struct s_v2f
{
	float_t			x;
	float_t			y;
}					t_v2f;

typedef struct s_v2i
{
	int32_t			x;
	int32_t			y;
}					t_v2i;

typedef struct s_player
{
	t_v2f			position;
	t_v2f			dir;
	t_v2f			plane;
}					t_player;

typedef struct s_textures
{
	char			*north;
	char			*south;
	char			*west;
	char			*east;
}					t_textures;

typedef struct s_colors
{
	int				floor_r;
	int				floor_g;
	int				floor_b;
	int				ceiling_r;
	int				ceiling_g;
	int				ceiling_b;
}					t_colors;

typedef struct s_map
{
	char			**grid;
	int				width;
	int				height;
	int				line_count;
	int				player_count;
	int				map_line_index;
	int				elements_count;
	mlx_texture_t	*tex_north;
	mlx_texture_t	*tex_south;
	mlx_texture_t	*tex_west;
	mlx_texture_t	*tex_east;
	t_textures		textures;
	t_colors		colors;
	mlx_image_t		*img;
	t_v2i			position;
	t_v2i			size;
	t_player		player;
}					t_map;

typedef struct s_game
{
	mlx_t			*mlx;
	mlx_image_t		*canvas;
	t_map			*map;
	int32_t			last_mouse_x;
	int32_t			last_mouse_y;
	bool			first_mouse;
}					t_game;

int					init_game(char *map_file);
t_map				*read_map(char *map_file);

// Map parsing functions
t_map				*init_map(void);
int					is_valid_element(char *line);
int					validate_map_line(char *line);
int					process_line(char *line, t_map *map, int fd);
int					validate_final_map(t_map *map);
// int     allocate_map_grid(t_map *map);
int					store_map_line(char *line, t_map *map);
int					load_map_data(char *map_file, t_map *map);
int					is_valid_map_char(char c);
int					check_first_last_row(t_map *map, int row);
int					check_side_walls(t_map *map);
int					is_valid_neighbor(t_map *map, int row, int col);
int					validate_walkable_spaces(t_map *map);
int					check_map_surrounded_by_walls(t_map *map);

// Texture and color loading functions
char				*extract_path(char *line);
int					parse_color(char *line, int *r, int *g, int *b);
int					load_texture(char *line, t_map *map);

// Player initialization functions
void				set_player_direction(t_player *player, char direction);
int					find_player_position(t_map *map, t_player *player);
int					init_player(t_map *map, t_player *player);

// Memory management functions
void				free_textures(t_map *map);
void				free_map_grid(t_map *map);
void				free_game(void);
void				free_map(void);
void				free_player(void);

// Map utility functions
void				fix_zero_space_to_zero(t_map *map);

// Mini map
int32_t				clamp(int32_t value, int32_t min, int32_t max);
int					minimap_setup(t_game *game);
void				minimap_render(t_game *game);
void				render_player(t_map *scene, uint32_t color);

// Engine
void				render_game(t_game *game);
void				draw_line(mlx_image_t *grid, t_v2f start, t_v2f end,
						uint32_t color);
void				draw_fov(t_game *game, t_map *scene, t_player *player);
bool				ft_at_wall(t_map *map, float_t offsetx, float_t offsety);
mlx_texture_t		*ft_get_texture(t_map *scene, uint32_t side, t_v2f step);
uint32_t			ft_darken_color(uint32_t color);
uint32_t			ft_get_pixel_color(uint8_t r, uint8_t g, uint8_t b,
						uint8_t a);
int32_t				ft_get_lineheight(float_t distance);

// Vector functions
t_v2i				veci_new(int32_t x, int32_t y);
t_v2i				veci_zero(void);
t_v2i				veci_from(t_v2i that);
t_v2i				veci_fromf(t_v2f that);
void				veci_print(char *label, t_v2i vec);
t_v2i				veci_scale(t_v2i vec, float_t factor);
t_v2i				veci_add(t_v2i vec, t_v2i that);
t_v2i				veci_sub(t_v2i vec, t_v2i that);
t_v2i				veci_div(t_v2i vec, t_v2i that);
t_v2i				veci_mul(t_v2i vec, t_v2i that);
t_v2f				vecf_rot(t_v2f vec, float_t angle);

t_v2f				vecf_new(float_t x, float_t y);
t_v2f				vecf_zero(void);
t_v2f				vecf_from(t_v2f that);
void				vecf_print(char *label, t_v2f vec);
t_v2f				vecf_scale(t_v2f vec, float_t factor);
t_v2f				vecf_scale(t_v2f vec, float_t factor);
t_v2f				vecf_add(t_v2f vec, t_v2f that);
t_v2f				vecf_sub(t_v2f vec, t_v2f that);
t_v2f				vecf_div(t_v2f vec, t_v2f that);
t_v2f				vecf_mul(t_v2f vec, t_v2f that);

// Window and game functions
int					init_window(t_game *game);
int					close_window(t_game *game);
int					handle_keypress(int keycode, t_game *game);
void				render_frame(t_game *game);
int					game_loop(t_game *game);
void				move_player_in_dir(t_game *game, t_v2f dir);
void				rotate_player(t_game *game, float_t rot_speed);

#endif
