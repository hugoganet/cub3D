# cub3D - Makefile (macOS, MiniLibX, libft)

NAME		:= cub3D

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror

INCDIR		:= includes
SRCDIR		:= src

LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a

### MiniLibX configuration (OpenGL only on macOS)
# Prefer local MLX checkout if present; override with MLX_DIR=/path/to/mlx
MLX_DIR		?= minilibx_opengl
MLX_LIB_SUB	:= $(MLX_DIR)/lib
MLX_INC		:= $(MLX_DIR)/include

# Search both MLX root and lib/ for the library
MLX_LDIRS	:= -L$(MLX_DIR) -L$(MLX_LIB_SUB)
MLX_FLAGS	:= $(MLX_LDIRS) -lmlx -framework OpenGL -framework AppKit

# Also include $(MLX_DIR) to handle MLX trees where mlx.h is at repo root
INCS		:= -I$(INCDIR) -I$(MLX_INC) -I$(MLX_DIR) -I$(LIBFT_DIR)/includes

SRC	:= \
	$(SRCDIR)/main.c \
	$(SRCDIR)/init.c \
	$(SRCDIR)/loop.c \
	$(SRCDIR)/input/keys.c \
	$(SRCDIR)/render/draw.c \
	$(SRCDIR)/render/textures.c \
	$(SRCDIR)/parser/parse_file.c \
	$(SRCDIR)/utils/errors.c \
	$(SRCDIR)/utils/mem.c

# Placeholders for future implementation (compiled but may be unused for now)
SRC += \
	$(SRCDIR)/render/raycast.c \
	$(SRCDIR)/render/dda.c \
	$(SRCDIR)/parser/parse_headers.c \
	$(SRCDIR)/parser/parse_map.c \
	$(SRCDIR)/parser/validate_map.c \
	$(SRCDIR)/parser/colors.c \
	$(SRCDIR)/utils/math.c

OBJ	:= $(SRC:.c=.o)

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

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -f $(OBJ)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME) maps/sample.cub

debug: CFLAGS += -g3 -fsanitize=address
debug: re
