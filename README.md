# Wolf3d (Software rasterizer)

A 3d software rasterizer engine written in C following 42-guidelines.
A raycast method is used for 3d effect.

The project was done by me (Vesa Eilo) and Okko Hakola as a part of the 42 curriculum.
I focused mainly on rendering which contained triangle rasterization and texture and normal mapping, and mesh handling. I also created the improvized "ray casting" projection method that allowed us to use triangle geometry without the use of projection matrices.

Features:

- Raycasted 3D
- Parallel rendering
- Depth shading
- Option for simple normal mapping
- Map editor
- `.obj` file import and usage for world generation
  - homemade 3d models :)
- AABB collision (player)
- AABB viewbox occulsion
- Skybox
- Minimap

![Gameplay](assets/img/screenshot1.png)

### Controls

```
WASD: Move
Mouse: Rotate
n: toggle normal maps
p: pause game
f: set full screen mode
esc: quit
Arrows+Enter: In menus, move and select
```

## Install

### Mac

```sh
git clone https://github.com/hakolao/wolf3d-rasterizer && cd wolf3d-rasterizer
make && ./wolf3d
```

### WSL

```
sudo apt update
sudo apt-get install xorg-dev # make sure x11 related stuff is installed
# install sdl
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-image-dev
# install vcxsrv on windows side & run it
export DISPLAY=:0
export LIBGL_ALWAYS_INDIRECT=1
# run wolf3d
git clone https://github.com/hakolao/wolf3d-rasterizer && cd wolf3d-rasterizer
make && ./wolf3d
```

### Linux

Install SDL2, SDL_image, SDL_ttf on linux.

```sh
sudo apt update
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-image-dev
```

Then play the game :)

```sh
git clone https://github.com/hakolao/wolf3d-rasterizer && cd wolf3d-rasterizer
make && ./wolf3d
```

### Map Editor

A simple map editor to generate levels for wolf3d.

![Mapeditor](assets/img/screenshot2.png)

```sh
make map_editor && ./wolf3d_editor #default size of 15x15
./wolf3d_editor --filename=maps/map_name_here --size=1-50
```

Add your map file to `src/scene/scene_data.c`

```c
	data->map_filename = ft_strdup("maps/level1");
```

Then recompile

```
make && ./wolf3d
```

## Notes

### External libraries:

- SDL: Window management, fonts, events, time and presenting the framebuffer
  - All graphics are software rendered
- Libc:
  - `pthreads.h` for parallelization
  - `stdlib.h`: `malloc`, `free`, `exit`
  - `stdarg.h`: variable length inputs
  - `stdint.h` and `float.h`: types
  - `fcntl.h`: `open`, `close`

### Libraries specifically made for this:

- lib3d: 2D and 3D functionality, rasterization, 3d object representation & reading etc.
- libgmatrix: matrix and vector math library
- libft/hash_map
- libft/thread_pool
