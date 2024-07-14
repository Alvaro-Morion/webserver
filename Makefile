# **************************************************************************** #
#                                                                              #
#                                                                              #
#    Makefile                                           :+:      :+:    :+:    #
#    Author:   Peru Riezu <riezumunozperu@gmail.com>                           #
#    github:   https://github.com/priezu-m                                     #
#    Licence:  GPLv3                                                           #
#    Created:  2023/09/27 18:57:07                                             #
#    Updated: 2024/07/14 12:29:11 by ahammoud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#GNU Make 3.81

################################################################################

SHELL :=			bash
CC :=				gcc
CXX :=				g++
CFLAGS :=			-O3 -flto -Wall -Wextra -Werror
CXXFLAGS :=			-O3 -flto -Wall -Wextra -Werror -std=c++98
CDEBUG_FLAGS :=		-O0 -g3 -flto -Wall -Wextra -Werror -fsanitize=address,undefined,leak
CXXDEBUG_FLAGS :=	-O0 -g3 -flto -Wall -Wextra -Werror -std=c++98 -fsanitize=address,undefined,leak
LDFLAGS :=			

################################################################################

NAME :=				webserver

DEP_PATH :=			./DEP
DEP_PATH_MAKE :=	DEP/
OBJ_PATH :=			./OBJ

EXCLUDE_DIRS :=		$(DEP_PATH) $(OBJ_PATH) ./.git ./example_website
EXCLUDE_FILES :=	./tags

CPPHDR :=			$(shell find . \
						$(foreach PATH, $(EXCLUDE_DIRS), -path "$(PATH)" -prune -o)\
						$(foreach FILE, $(EXCLUDE_FILES), -path "$(FILE)" -prune -o)\
						-type f -and -name "*.hpp" -print)
CPPHDR :=				$(CPPHDR:./%=%)
CPPSRC :=			$(shell find . \
						$(foreach PATH, $(EXCLUDE_DIRS), -path "$(PATH)" -prune -o)\
						$(foreach FILE, $(EXCLUDE_FILES), -path "$(FILE)" -prune -o)\
						-type f -and -name "*.cpp" -print)
CPPSRC :=				$(CPPSRC:./%=%)
CHDR :=				$(shell find . \
						$(foreach PATH, $(EXCLUDE_DIRS), -path "$(PATH)" -prune -o)\
						$(foreach FILE, $(EXCLUDE_FILES), -path "$(FILE)" -prune -o)\
						-type f -and -name "*.h" -print)
CHDR :=				$(CHDR:./%=%)
CSRC :=				$(shell find . \
						$(foreach PATH, $(EXCLUDE_DIRS), -path "$(PATH)" -prune -o)\
						$(foreach FILE, $(EXCLUDE_FILES), -path "$(FILE)" -prune -o)\
						-type f -and -name "*.c" -print)
CSRC :=				$(CSRC:./%=%)
OBJ :=				$(addprefix $(OBJ_PATH)/,$(CSRC:%.c=%.o))
OBJ +=				$(addprefix $(OBJ_PATH)/,$(CPPSRC:%.cpp=%.o))
DEP :=				$(addprefix $(DEP_PATH)/,$(CSRC:.c=.d))
DEP +=				$(addprefix $(DEP_PATH)/,$(CPPSRC:.cpp=.d))
SUB_DIR :=			$(sort $(dir $(CSRC)))
SUB_DIR +=			$(sort $(dir $(CPPSRC)))
SUB_DIR :=			$(SUB_DIR:.%=%)
SUB_DIR :=			$(SUB_DIR:/%=%)
NEW_DIRS :=			$(addprefix $(OBJ_PATH)/,$(SUB_DIR))
NEW_DIRS +=			$(addprefix $(DEP_PATH)/,$(SUB_DIR))
NEW_DIRS +=			$(OBJ_PATH) $(DEP_PATH)

CURRENT_DIR :=		$(shell pwd)
MANPATH_APPEND :=	$(CURRENT_DIR)/manpages
CURRENT_MANPAHT :=	$(shell man --path)

ifeq ($(MAKECMDGOALS),$(NAME))
-include $(DEP)
endif

ifeq ($(MAKECMDGOALS),all)
-include $(DEP)
endif

ifeq ($(MAKECMDGOALS),)
-include $(DEP)
endif

################################################################################

.DEFAULT_GOAL :=	$(NAME)

$(NEW_DIRS):
	@mkdir -p $@

$(DEP_PATH)/%.d: %.cpp | $(NEW_DIRS)
	@rm -f $(DEP_PATH)/$@; \
		$(CXX) $(CFLAGS) -M $< > $@.tmp; \
		sed 's,$(notdir $*).o[ :]*,$(OBJ_PATH)/$(subst $(DEP_PATH_MAKE),,$(basename $@).o) $@ : ,g' \
	   	< $@.tmp > $@; \
		rm -f $@.tmp

$(DEP_PATH)/%.d: %.c | $(NEW_DIRS)
	@rm -f $(DEP_PATH)/$@; \
		$(CC) $(CFLAGS) -M $< > $@.tmp; \
		sed 's,$(notdir $*).o[ :]*,$(OBJ_PATH)/$(subst $(DEP_PATH_MAKE),,$(basename $@).o) $@ : ,g' \
	   	< $@.tmp > $@; \
		rm -f $@.tmp

$(OBJ_PATH)/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(CC) $(CFLAGS) -c $< -o $@\n"

$(OBJ_PATH)/%.o: %.cpp
	@$(CXX) $(CFLAGS) -c $< -o $@
	@printf "$(CXX) $(CXXFLAGS) -c $< -o $@\n"

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)
	make -C ./example_website/cgi/

.PHONY: all clean fclean re push update_manpath tags debug format

all:
	@make --no-print-directory $(NAME)

clean:
	@rm $(DEP) $(OBJ) debug &> /dev/null || true
	@rmdir -p $(NEW_DIRS) $(DEP_PATH) $(OBJ_PATH) &> /dev/null || true

fclean: clean
	@rm $(NAME) &> /dev/null || true

re: fclean
	@make --no-print-directory

pull:
	@git pull --rebase -X theirs

push: fclean
	@git add .
	@git commit
	@git push

update_manpath:
	@printf "execute:\n"
	@printf "export MANPATH='$(CURRENT_MANPAHT):$(MANPATH_APPEND)'\n"
	@printf "to update the manpath and become able to read the manuals\n"

tags:
	@ctags --extras-all=* --fields-all=* --c-kinds=* --c++-kinds=* $(CSRC) $(CHDR) $(CPPSRC) $(CPPHDR)

debug:
	@make --no-print-directory re CFLAGS="$(CDEBUG_FLAGS)" CXXFLAGS="$(CXXDEBUG_FLAGS)"
	@make --no-print-directory clean
	@mv $(NAME) debug

format:
	clang-format -i $(CSRC) $(CHDR) $(CPPSRC) $(CPPHDR)
