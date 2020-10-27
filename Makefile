CC = gcc
NAME=wolf3d
DIR_SRC = ./src
DIR_OBJ = temp
LIBSDL2 = ./lib/SDL2
LIBFT = ./lib/libft
LIB3D = ./lib/lib3d
LIBGMATRIX = ./lib/libgmatrix

LIBFTFLAGS = -L$(LIBFT) -lft
LIB3DFLAGS = -L$(LIB3D) -l3d
LIBGMATRIXFLAGS = -L$(LIBGMATRIX) -lgmatrix
SDL_FLAGS = -rpath $(LIBSDL2) \
				-framework SDL2 -F$(LIBSDL2)/ \
				-framework SDL2_image -F$(LIBSDL2)/ \
				-framework SDL2_ttf -F$(LIBSDL2)/
LIBS = $(LIBFTFLAGS) $(LIB3DFLAGS) $(LIBGMATRIXFLAGS) $(SDL_FLAGS)
INCLUDES = -I ./include \
		-I$(LIBFT)/include \
		-I$(LIB3D)/include \
		-I$(LIBGMATRIX)/include \
		-I$(LIBSDL2)/SDL2.framework/Headers \
		-I$(LIBSDL2)/SDL2_image.framework/Headers \
		-I$(LIBSDL2)/SDL2_ttf.framework/Headers

FLAGS = -Wall -Wextra -Werror -O3
SOURCES = main.c \
			wolf3d.c \
			player.c \
			camera.c \
			scene.c \
			time.c \
			debug.c \
			render/render.c \
			render/render_utils.c \
			window/ui.c \
			window/text.c \
			window/window.c \
			window/utils.c \
			events/mouse.c \
			events/keyboard.c \

SRCS = $(addprefix $(DIR_SRC)/,$(SOURCES))
OBJS = $(addprefix $(DIR_OBJ)/,$(SOURCES:.c=.o))

all: $(DIR_OBJ) $(NAME)

$(NAME): $(OBJS)
	@printf "\033[32;1mCompiling libs...\n\033[0m"
	make -C $(LIBFT)
	make -C $(LIB3D)
	make -C $(LIBGMATRIX)
	@printf "\033[32;1mCompiling app...\n\033[0m"
	$(CC) $(FLAGS) $(LIBS) -o $@ $^
	@printf "\033[32;1mDone. Run: ./$(NAME)\n\033[0m"

$(DIR_OBJ):
	@mkdir -p temp
	@mkdir -p temp/window
	@mkdir -p temp/render
	@mkdir -p temp/events

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(CC) $(FLAGS) $(INCLUDES) -c -o $@ $<

clean:
	@make -C $(LIBFT) clean
	@make -C $(LIB3D) clean
	@make -C $(LIBGMATRIX) clean
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)

fclean: clean
	@make -C $(LIBFT) fclean
	@make -C $(LIB3D) fclean
	@make -C $(LIBGMATRIX) fclean
	@/bin/rm -f $(NAME)

re: fclean all

testrun: #this is only for quicker lib3d debug
	#@make -C $(LIB3D) clean
	#make -C $(LIB3D)
	@/bin/rm -f $(OBJS)
	@/bin/rm -rf $(DIR_OBJ)
	make all
	./wolf3d

norm: norminette $(DIR_SRC) $(LIBFT) $(LIB3D) $(LIBGMATRIX) ./include

.PHONY: all, $(DIR_OBJ), clean, fclean
