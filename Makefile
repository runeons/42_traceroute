# clang -fsanitize=address -g3 srcs/*.c -Wall -Wextra -Werror -I includes/ -I libft/includes/ -I libft -L libft -lft && sudo ./a.out 8.8.8.8

# *** NAME - CC - FLAGS ****************************************************** #

NAME					= 	ft_traceroute

CC						= 	gcc -g3

FFLAGS					=   -fsanitize=address

LFLAGS					=   -fsanitize=leak

CFLAGS					= 	-I includes/ \
							-I libft/includes/ \
							-Wall -Wextra -Werror 

INC_LIB						= 	-I libft
LIB							= 	-L libft -lft



# *** PROJECT HEADER ********************************************************* #

HDIR					=	$(addprefix -I, $(HEADER_DIR))

HEAD					=	$(addprefix $(HEADER_DIR), $(HEADER))

HEADER_DIR				=	\
							./includes/

HEADER					=	\
							traceroute_data.h \
							traceroute_functions.h \
							utils_colors.h \
							utils_constants.h \
							utils_options.h \


# *** SRCS ******************************************************************* #

SRCS_DIR					=	./srcs/

SRCS_LIST					=	\
							craft_icmp.c \
							craft_udp.c \
							display.c \
							init_data.c \
							init_socket.c \
							init_options.c \
							main.c \
							traceroute.c \
							utils_debug.c \
							utils_libft.c \
							utils_options.c \
							utils_time.c \

SRCS					=	$(addprefix $(SRCS_DIR), $(SRCS_LIST))

# *** OBJS ******************************************************************* #

OBJS_DIR				=	./objs/

OBJS_LIST				=	$(patsubst %.c, %.o, $(SRCS_LIST))

OBJS					=	$(addprefix $(OBJS_DIR), $(OBJS_LIST))



all: $(OBJS) $(SRCS) $(NAME)

$(NAME): $(OBJS) $(SRCS)
	@ make -C libft/
	@ $(CC) $(CFLAGS) $(HDIR) $(OBJS) $(INC_LIB) $(LIB) -o $@

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $(HDIR) -D BONUS=$(BONUS_FLAG) $(INC_LIB) -c -o $@ $<

fsan:  $(OBJS) $(SRCS)
	@ make -C libft/
	@ $(CC) $(CFLAGS) $(FFLAGS) $(HDIR) $(OBJS) $(INC_LIB) $(LIB) -o $(NAME)

leak:  $(OBJS) $(SRCS)
	@ make -C libft/
	@ $(CC) $(CFLAGS) $(LFLAGS) $(HDIR) $(OBJS) $(INC_LIB) $(LIB) -o $(NAME)

clean:
	@ make clean -C libft/
	@ rm -rf $(OBJS_DIR)

fclean:
	@ make fclean -C libft/
	@ rm -f $(NAME)
	@ rm -rf $(OBJS_DIR)

re: fclean all

fsanr: fclean fsan

.PHONY: all clean fclean re bonus fsan fsanre
