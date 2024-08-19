#Compiler and Linker
CC          := cc

#The Target Binary Program
TARGET      := philo

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := ../philo/src
INCDIR      := ../philo/inc
OBJDIR      := ../philo/build/obj
TARGETDIR   := .
BUILDIR     := ../philo/build
RESDIR      := res
SRCEXT      := c
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -I../philo/inc -Wall -Werror -Wextra 

DEBUG_FLAGS := -g3 -fsanitize=thread


#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := ../philo/src/main.c ../philo/src/init/init.c ../philo/src/routine/monitor.c ../philo/src/routine/routine_utils.c \
			../philo/src/routine/routine.c ../philo/src/routine/threads.c ../philo/src/routine/wait.c ../philo/src/utils/error.c \
			../philo/src/utils/lib.c ../philo/src/utils/time.c
OBJECTS     := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Default Make

all: build $(TARGETDIR)/$(TARGET) 

#Remake
re: fclean all

#Make the Directories
build:
	@mkdir -p $(OBJDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(OBJDIR)

#Full Clean, Objects and Binaries
fclean: clean
	@$(RM) -rf $(BUILDIR)
	@$(RM) -rf $(TARGET)

#Link
$(TARGETDIR)/$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o  $(TARGETDIR)/$(TARGET)

#Compile
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

#Non-File Targets
.PHONY: all re clean fclean
