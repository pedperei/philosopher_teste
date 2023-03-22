
# Variáveis

NAME		= philo
CC			= cc
CFLAGS		= -g -pthread -fsanitize=thread
RM			= rm -f

# Cores

DEFAULT = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

#Sources

SRC_FILES	=	testes utils

all: $(NAME) 

$(NAME):
	@echo "$(YELLOW)Compiling..."
	@$(CC) -c $(CFLAGS) $(SRC_FILES:=.c)
	@$(CC) $(CFLAGS) $(SRC_FILES:=.o) -o $(NAME)
	@echo "$(GREEN)Philosophers compilled!$(DEFAULT)"

clean:
	@echo "$(RED)Deleted .o files"
	@$(RM) $(SRC_FILES:=.o)

fclean:	clean
	@$(RM) -f $(NAME)
	@echo "$(RED)Deleted Philo"

re:			fclean all
			@echo "$(BLUE)Code cleaned and rebuilt$(DEFAULT)"

.PHONY: all clean fclean re