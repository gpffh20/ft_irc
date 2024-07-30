.PHONY: all clean fclean re mem dsym

BLACK := $(shell tput -Txterm setaf 0)
RED := $(shell tput -Txterm setaf 1)
GREEN := $(shell tput -Txterm setaf 2)
YELLOW := $(shell tput -Txterm setaf 3)
LIGHTPURPLE := $(shell tput -Txterm setaf 4)
PURPLE := $(shell tput -Txterm setaf 5)
BLUE := $(shell tput -Txterm setaf 6)
WHITE := $(shell tput -Txterm setaf 7)
RESET := $(shell tput -Txterm sgr0)

NAME = ircserv

RM = rm -f
MKDIR = mkdir -p

CXX = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98
SRC_DIR = src/
INC_DIR = inc/
OBJ_DIR = obj/

SRC_FILES = main.cpp \
			client.cpp \
			Message.cpp \
			server/server.cpp \
			cmd/command.cpp \
			cmd/invite.cpp \
			cmd/join.cpp \
			cmd/kick.cpp \
			cmd/mode.cpp \
			cmd/nick.cpp \
			cmd/pass.cpp \
			cmd/ping.cpp \
			cmd/privmsg.cpp \
			cmd/quit.cpp \
			cmd/user.cpp

SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJS = $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRCS))

ifneq "$(findstring mem, $(MAKECMDGOALS))" ""
	CFLAGS += -fsanitize=address -g3
else ifneq "$(findstring dsym, $(MAKECMDGOALS))" ""
	CFLAGS += -g3
endif

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CFLAGS) -o $@ $^
	@echo "$(GREEN)Executable file created!$(RESET)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@echo "$(PURPLE)Compiling:$(RESET) $<"
	@$(MKDIR) $(dir $@)
	@$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	@$(RM) -r $(OBJ_DIR)
	@echo "$(GREEN)DONE!$(RESET)"

fclean: clean
	@echo "$(YELLOW)Deleting executable file...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(GREEN)DONE!$(RESET)"

re: fclean all

mem: fclean all

dsym: fclean all
