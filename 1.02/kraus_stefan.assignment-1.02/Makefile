¥CC = gcc
CXX = g++
ECHO = echo
RM = rm -f

CFLAGS = -Wall -ggdb -funroll-loops
CXXFLAGS = -Wall -ggdb -funroll-loops
LDFLAGS = 

BIN = rlg327
OBJS = rlg327.o heap.o

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

.PHONY: all clean clobber etags

clean:
	@$(ECHO) Removing all generated files
	@$(RM) *.o $(BIN) *.d TAGS core vgcore.*

clobber: clean
	@$(ECHO) Removing backup files
	@$(RM) *~ \#*

etags:
	@$(ECHO) Updating TAGS
	@etags *.[ch]
