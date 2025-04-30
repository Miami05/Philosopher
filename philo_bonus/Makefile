# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 18:14:56 by ldurmish          #+#    #+#              #
#    Updated: 2025/04/30 18:43:33 by ldurmish         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name
NAME_BONUS = philo_bonus

# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-fsanitize=thread #-pthread
INCLUDE = -I include/

# Directories
BONUS = src/

# Color Codes
RESET = \033[0m
RED = \033[31m
GREEN = \033[32m

# Bonus files for philosopher
BONUS_SRCS = $(BONUS)main_bonus.c $(BONUS)utils_bonus.c $(BONUS)arguments_bonus.c \
			 $(BONUS)init_bonus.c $(BONUS)philosopher_routine_bonus.c \

# Object files
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# Main target
all: bonus

# Bonus files to compile philosopher
bonus: $(BONUS_OBJS)
	@echo "$(GREEN)🔧 Compiling Philosopher bonus...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDE) $(BONUS_OBJS) -o $(NAME_BONUS)
	@echo "$(GREEN)✅ Philosopher bonus compiled Successfully!$(RESET)"

# Compile individual object files
%.o: %.c
	@echo "$(GREEN)📦 Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
# Clean object files and executables
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(BONUS_OBJS)
	@echo "$(GREEN)🧹 Object and extra builds cleaned!$(RESET)"

# Removing all including executables
fclean: clean
	@echo "$(RED)Cleaning executables...$(RESET)"
	@rm -f $(NAME_BONUS)
	@echo "$(GREEN)🧼 Everything cleaned!$(RESET)"

# Cheking leaks with vlagrind and helgrind
valgrind: $(NAME_BONUS)
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME_BONUS) $(ARGS)
helgrind: $(NAME_BONUS)
		valgrind --tool=helgrind --history-level=none ./$(NAME_BONUS) $(ARGS)

# Rebuild the project
re: fclean all

# Phony targets
.PHONY: all clean fclean re bonus

