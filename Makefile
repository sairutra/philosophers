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
CFLAGS      := -Iphilo/inc -Wall -Werror -Wextra -g3 -fsanitize=thread

DEBUG_FLAGS := -g3 -fsanitize=thread


#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := philo/src/main.c
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

#Full Clean, Objects and Binaries
fclean: clean
	@$(RM) -rf $(BUILDIR)

#Link
$(TARGETDIR)/$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o  $(TARGETDIR)/$(TARGET) $(DEBUG_FLAGS)
#Compile
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

#Non-File Targets
.PHONY: all re clean fclean
