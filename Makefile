# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldurmish < ldurmish@student.42wolfsburg.d  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/17 21:13:08 by ldurmish          #+#    #+#              #
#    Updated: 2025/04/18 02:46:58 by ldurmish         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name
NAME = philo
NAME_BONUS = philo_bonus

# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread -pthread
INCLUDE = -I include/

# Directories
SRC = src/

# Color Codes
RESET = \033[0m
RED = \033[31m
GREEN = \033[32m

# Source files for philosopher
SRCS = $(SRC)main.c $(SRC)arguments.c $(SRC)utils.c $(SRC)death.c

# Object files
OBJS = $(SRCS:.c=.o)

# Main target
all: $(NAME)

# Compiling philosopher
$(NAME): $(OBJS)
	@echo "$(GREEN)🔧 Compiling Philosopher...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ Philosopher Compiled Successfully!"

# Compile individual object files
%.o: %.c
	@echo "$(GREEN)📦 Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean object files and executables
clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS) asan_$(NAME) helgrind_$(NAME)
	@echo "$(GREEN)🧹 Object and extra builds cleaned!$(RESET)"

# Removing all including executables
fclean: clean
	@echo "$(RED)Cleaning executables...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)🧼 Everything cleaned!$(RESET)"

# Rebuild the project
re: fclean all

# Phony targets
.PHONY: all clean fclean re bonus
