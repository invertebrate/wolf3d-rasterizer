/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 15:06:23 by ohakola           #+#    #+#             */
/*   Updated: 2020/09/02 15:16:12 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include <SDL.h>
# include <SDL_image.h>
# include "libft.h"
# include "libgmatrix.h"
# include "lib3d.h"

# define EXIT_FAILURE 1
# define EXIT_SUCCESS 0

# define WIDTH 1280 / 4
# define HEIGHT 720 / 4

/*
**	The view scale will scale the camera and raycasting in relation to the
**	world and objects. So that bigger scale will effectively scale the "world"
**	smaller in the camera view without affecting the actual world.
*/

# define VIEW_SCALE 1
# define FPS 60
# define NAME "Wolf3D"

/*
**	The vector convention is a right handed coordinate system where up axis 
**	is z (2), left axis is y (1), forward axis is x (0). The numbers are for
**	easier referencing in t_vec3 structs.
*/

# define VEC_UP 2
# define VEC_LEFT 1
# define VEC_FORWARD 0

/*
**	Frame buffer
*/

# define BYTES_PER_PIXEL 8

typedef enum						e_move
{
	forward,
	backward,
	strafe_left,
	strafe_right
}									t_move;

typedef struct						s_window
{
	SDL_Renderer			*renderer;
	SDL_Texture				*frame;
	uint32_t				*framebuffer;
	int32_t					width;
	int32_t					height;
	int32_t					pitch;
	t_bool					resized;
	SDL_Window				*window;
	uint32_t				window_id;
	t_bool					is_hidden;
	void					*parent;
}									t_window;

/*
**	Camera screen distance is the distance of the
**	virtual, screen through which the rays are projected,
**	from the camera in world units. The virtual screen dimensions
**	and the screen distance determine the field of view and vice versa.
*/

typedef struct						s_camera
{
	t_vec3					origin;
	t_vec3					orientation[3];
	uint16_t				screen_width;
	uint16_t				screen_height;
	float					fovx;
	float					fovy;
	float					screen_dist;
	t_ray					*rays;
	int						raycount;
	uint32_t				*framebuffer;
	struct s_scene			*parent_scene;

	t_mat4					view;
	t_mat4					model;
	t_mat4					projection;
}									t_camera;

typedef struct						s_player
{
	t_vec3					pos;
	t_vec3					forward;
	t_vec3					up;
	float					speed;
	float					rot_speed;
	t_camera				camera;
}									t_player;

//ToDo: Object to contain mesh(es) from lib3d

/*
**	Structs and typedefs used in rendering
*/

typedef void *(t_vertex_shader(char *fmt, ...));
typedef void *(t_fragment_shader(char *fmt, ...));

typedef struct						s_shader
{
	t_vertex_shader			*v;
	t_fragment_shader		*f;
}									t_shader;

typedef struct						s_mesh
{
	t_vertex				**vtc;
	int						vtx_count;
	t_triangle				*triangles;
	int						triangle_count;
	t_vec3					origin;
	t_vec3					orientation[3];
	t_box3d					bound_box;
	t_shader				shader;
	struct s_object			*parent_object;
}									t_mesh;

typedef struct						s_object
{
	t_mesh					*mesh;
	t_vec3					origin;
	t_vec3					orientation[3];
	t_vec3					parent_origin;
	t_vec3					parent_orientation[3];
	struct s_scene			*parent_scene;
}									t_object;

/*
**	Typedefs related to app and scene management
*/

typedef struct						s_scenedata
{
	int						level;
	//add here all the date needed to create a scene
	//for example fetch the map and included
	//objects. This will be passed to new_scene()
	//that will interpret the data and create the scene
}									t_scene_data;

typedef struct						s_scene
{
	t_object				**objects;
	uint32_t				object_count;
	t_camera				*main_camera;
	t_window				*main_window;
}									t_scene;

typedef struct						s_wolf3d
{
	int						start_time;
	t_bool					is_running;
	t_window				*main_window;
	t_player				player;
	t_scene					*active_scene;
}									t_wolf3d;

/*
**	Function declarations
*/

void								wolf3d_run(t_wolf3d *app);
void								cap_framerate(uint32_t start_time);

/*
** Scene
*/

t_scene								*new_scene(t_wolf3d *app,
												t_scene_data *data);
void								destroy_scene(t_wolf3d *app);

/*
** Objects
*/

t_object							*create_object_triangle(t_scene *scene,
															t_wolf3d *app);

/*
** Player
*/

void								init_player(t_wolf3d *app);
void								move_player(t_player *player, t_move dir);

/*
** Camera
*/

t_camera							*new_camera(t_scene *scene,
												float screen_distance);
void								update_camera_view(t_player *player);
void								init_camera(t_player *player);
void								camera_transform(t_camera *camera,
									t_vec4 vertex, t_vec4 res);

/*
** Draw / Render
*/

void								draw_frame(t_wolf3d *app);
t_bool								render_mesh(t_mesh *mesh,
												t_camera *camera);
t_bool								render_triangle(t_triangle *triangle,
													t_mesh *mesh,
													t_camera *camera);
int									screen_to_frame_coords(t_scene *scene,
															int x, int y);

/*
** Utils
*/

void								error_check(int test, const char *message);

/*
** Window
*/

void								main_window_init(t_wolf3d *app);

#endif