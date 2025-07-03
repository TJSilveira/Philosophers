GREEN		=	\033[1;32m
LIGHT_GREEN	=	\033[0;32m
RED			=	\033[1;31m
YELLOW		=	\033[0;33m
ITALIC		=	\033[3m
RESET		=	\033[0m

NAME = philo

CC = gcc

CFLAGS = -Wall -Werror -Wextra

INCLUDE = -I ./includes/

#Directories
SRC_DIR			= ./src/
SRC_DIR_BONUS	= ./src_bonus/
OBJS_DIR		= ./objs/
BIN_DIR			= ./bin/

# Source Files
SRC_FILES = philo.c \
			actions.c \
			init_functions.c \
			utils_1.c \
			utils_2.c

# Object Files
OBJS_FILES			= $(SRC_FILES:.c=.o)
OBJS				= $(addprefix $(OBJS_DIR), $(OBJS_FILES))

# Binary File
BIN					= $(addprefix $(BIN_DIR), $(NAME))

all: ${NAME}

# creating executables
${NAME}: ${OBJS_DIR} ${BIN_DIR} ${OBJS}
	@echo ""
	@echo "$(YELLOW) Preparing philo... $(RESET)"
	@${CC} ${CFLAGS} ${OBJS} -o ${BIN}
	@echo "$(LIGHT_GREEN) philo successfully compiled.$(RESET)"

$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	${CC} ${CFLAGS} -c ${INCLUDE} $< -o $@

# Creating Objects dir
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Creating Objects dir
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	@echo ""
	@echo "$(RED)Did someone call - $(ITALIC)pause$(RESET)$(RED) - the Clean-up crew?!$(RESET)"
	@$ rm -rf $(OBJS_DIR)

valgrind:
	valgrind ./bin/philo 100 80 10 10 50
	valgrind ./bin/philo 1 10 1 10 50

fclean: clean
	rm -rf $(BIN_DIR)

re: fclean $(NAME)

.PHONY: all clean fclean re
