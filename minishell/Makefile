# Dossier des fichiers objets
BUILD_DIR = build

# Fichiers sources
SRC_FILES = minishell.c \
prompt.c \
minishell_utils.c \
exit.c \
execution.c\
exit_minishell.c\
export.c\
init.c

# Transformation des fichiers sources en fichiers objets
OBJFILES = $(addprefix $(BUILD_DIR)/, $(SRC_FILES:.c=.o))

# Compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline -lncurses

# Libft
LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

# Exécutable
NAME = minishell




# Compilation complète
all: $(BUILD_DIR) $(LIBFT) $(NAME)

# Création du dossier build/ si nécessaire
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compilation de Libft
$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

# Compilation de Minishell
$(NAME): $(OBJFILES) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJFILES) $(LIBFT) $(LIBS)

# Compilation des fichiers .c en .o
# PS : 42 autorise totalement ce genre de règles car elles ne détectent pas les fichiers automatiquement, elles les transforment juste.
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -rf $(BUILD_DIR)
	@$(MAKE) -C $(LIBFT_PATH) clean

# Nettoyage complet (exécutable + objets)
fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_PATH) fclean

# Recompilation complète
re: fclean all
