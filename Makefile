# cub3D - Makefile (macOS, MiniLibX, libft)

NAME		:= cub3D

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror

INCDIR		:= includes
SRCDIR		:= src

LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a

BUILDDIR	:= build
OBJDIR		:= $(BUILDDIR)/obj

MLX_DIR		?= minilibx-linux
MLX_LIB_SUB	:= $(MLX_DIR)
MLX_INC		:= $(MLX_DIR)

MLX_LDIRS	:= -L$(MLX_DIR)
MLX_FLAGS	:= $(MLX_LDIRS) -lmlx -lXext -lX11 -lm
INCS		:= -I$(INCDIR) -I$(MLX_INC) -I$(MLX_DIR)

# Also include $(MLX_DIR) to handle MLX trees where mlx.h is at repo root
INCS := -I$(INCDIR) -I$(MLX_INC) -I$(MLX_DIR) -I$(LIBFT_DIR)/includes -I$(LIBFT_DIR)

SRC	:= \
	$(SRCDIR)/main.c \
	$(SRCDIR)/init.c \
	$(SRCDIR)/loop.c \
	$(SRCDIR)/input/keys.c \
	$(SRCDIR)/input/movement.c \
	$(SRCDIR)/render/draw.c \
	$(SRCDIR)/render/minimap.c \
	$(SRCDIR)/render/minimap_utils.c \
	$(SRCDIR)/render/textures.c \
	$(SRCDIR)/render/texture_utils.c \
	$(SRCDIR)/render/texture_cleanup.c \
	$(SRCDIR)/render/background.c \
	$(SRCDIR)/parser/check_map_closed.c \
	$(SRCDIR)/parser/find_player.c \
	$(SRCDIR)/parser/map_neighbors.c \
	$(SRCDIR)/parser/validate_chars.c \
	$(SRCDIR)/parser/validate_player.c \
	$(SRCDIR)/parser/parse_file.c \
	$(SRCDIR)/parser/parse_input.c \
	$(SRCDIR)/parser/parse_tex.c \
	$(SRCDIR)/parser/parse_map.c \
	$(SRCDIR)/parser/parse_color.c \
	$(SRCDIR)/parser/parsing_utils.c \
	$(SRCDIR)/utils/errors.c \
	$(SRCDIR)/utils/mem.c

# Placeholders for future implementation (compiled but may be unused for now)
SRC += \
	$(SRCDIR)/render/raycasting.c \
	$(SRCDIR)/render/projection.c \
	$(SRCDIR)/render/projection_utils.c \
	$(SRCDIR)/render/dda.c \
	$(SRCDIR)/utils/math.c

# Include missing render utility sources
SRC += \
	$(SRCDIR)/render/draw_utils.c \
	$(SRCDIR)/render/ray_utils.c

OBJ	:= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

.PHONY: all clean fclean re run debug mlx

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): mlx $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX_FLAGS) -o $(NAME)
	@echo "Built $(NAME)"

mlx:
	@if [ -f "$(MLX_DIR)/Makefile" ]; then \
		$(MAKE) -C $(MLX_DIR); \
	else \
		echo "[INFO] No local MiniLibX at '$(MLX_DIR)'. If using system MLX, set MLX_DIR accordingly."; \
	fi

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJDIR)
	@echo "Cleaned object files"

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -rf $(BUILDDIR)
	@rm -f $(NAME)
	@echo "Cleaned build directory and binary"

re: fclean all

run: $(NAME)
	./$(NAME) maps/sample.cub

debug: CFLAGS += -g3 -fsanitize=address
debug: re
