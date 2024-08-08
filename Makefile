include sources.mk

#Compiler and Linker
CC          := cc

#The Target Binary Program
TARGET      := philo

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := philo/src
INCDIR      := philo/inc
OBJDIR      := build/obj
TARGETDIR   := build/bin
BUILDIR     := build
RESDIR      := res
SRCEXT      := c
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -Wall -Werror -Wextra
LIB         := lib
LIBFT       := libft
LIBFT.A     := libft.a
DEBUG_FLAGS := -fsanitize=address -g


#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
OBJECTS     := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Default Make

all: build $(TARGETDIR)/$(TARGET) 

#Remake
re: fclean all

#Make the Directories
build:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(OBJDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(OBJDIR)
	@$(MAKE) -C $(LIB)/$(LIBFT) clean
	@$(MAKE) -C $(LIB)/$(MLX) clean

#Full Clean, Objects and Binaries
fclean: clean
	@$(RM) -rf $(BUILDIR)
	@$(MAKE) -C $(LIB)/$(LIBFT) fclean
	@$(MAKE) -C $(LIB)/$(MLX) clean

#Link
$(TARGETDIR)/$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o  $(TARGETDIR)/$(TARGET) 

#Compile
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^


#Non-File Targets
.PHONY: all re clean fclean
