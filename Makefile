
#Colors:
########################
BOLD = \033[1m
BLUE = \033[0;34m
CYAN = \033[36m
GREEN = \033[32m
ORANGE = \033[93m
PURPLE = \033[0;95m
RED = \033[0;91m
END = \033[0m
########################

NAME	        =		philo

SRCS            =       srcs/philo.c\
						srcs/utils.c\
						srcs/routine.c

INCLUDES_DIR     =       includes/

CC              =       gcc

OBJS            =       ${SRCS:.c=.o}

RM          =       rm -rf

FLAGS       =       -Wall -Wextra -Werror -pthread -g3

%.o : %.c ./includes/push_swap.h
						@${CC} ${FLAGS} -I${INCLUDES_DIR} -c $< -o ${<:.c=.o}
						@echo "\t\t${BOLD}${GREEN}[LOADED]${END} ${PURPLE}${<:.s=.o}${END}"

all:			${NAME}

${NAME}:		${OBJS}
						@${CC} ${FLAGS} -I${INCLUDES_DIR} -o ${NAME} ${OBJS}
						@echo "\t\t${BOLD}${CYAN}philo${END}   ${GREEN}[ OK ]${END}"

clean:
						@${RM} ${OBJS} ${OBJS_B}
						@echo "\n${BOLD}${RED}\t\t *.o files deleted.${END}"

fclean:		clean
						@${RM} ${NAME} ${NAME_B}
						@echo "\t\t${BOLD}${RED}Program philo deleted.${END}"

re:		fclean all

.PHONY:           all     clean   fclean  re
