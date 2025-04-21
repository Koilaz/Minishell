# Dossier des fichiers objets
BUILD_DIR = build

# Fichiers sources
SRC_FILES = minishell.c \
parsing/initialization/minishell_init.c \
parsing/initialization/env_minimal_init.c \
parsing/initialization/env_completion.c \
parsing/initialization/shlvl.c \
parsing/initialization/signal.c \
parsing/prompt/prompt.c \
parsing/prompt/hostname.c \
parsing/lexer/lexer.c \
parsing/lexer/word_segments.c \
parsing/lexer/word_fragments.c \
parsing/lexer/operator_segments.c \
parsing/lexer/segment_list.c \
parsing/lexer/segment_list_free.c \
parsing/lexer/lexer_utils.c \
parsing/expansion/expand_env.c \
built_in/exit.c \
built_in/export.c \
built_in/echo.c \
built_in/unset.c \
built_in/env.c \
built_in/pwd.c \
built_in/cd.c \
execution/execution.c\
execution/init_redirection.c\
execution/redirection.c\
execution/execution_utils.c\
utils/minishell_utils.c \
exit_minishell.c \

# Transformation des fichiers sources en fichiers objets
OBJFILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

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
	@mkdir -p $(dir $@)
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
