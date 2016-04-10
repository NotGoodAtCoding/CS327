CC = gcc
ECHO = echo
RM = rm -f

CFLAGS = -Wall -Werror -ggdb
CXXFLAGS = -Wall -Werror -ggdb
LDFLAGS = -lm -lncurses -lc

BIN = rlg229
OBJS = main.o dungeon.o p_queue.o room.o player.o point.o color.o monster.o dice_param.o ability.o parser.o object.o object_type.o object_c.o prompt.o linked_list.o stack.o c_dice.o

all: $(BIN) etags

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

%.o: %.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -MMD -MF $*.d -c $<



.PHONY: clean clobber etags

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) *.d TAGS core vgcore.*

clobber: clean
	@$(ECHO) Removing backup files
	@$(RM) *~ \#*

etags:
	@$(ECHO) Updating TAGS
	@etags *.[ch]
