CFLAGS = -g -Wall -Werror 

OBJS = main.o \
character.o \
core.o \
debug.o \
dijkstra.o \
dungeon.o \
heap.o \
move.o \
npc.o \
path.o \
pc.o \
point.o \
room.o \
turn.o \
ui.o \
util.o

NAME = rlg327

all: $(NAME)

clean:
	rm $(NAME) *.o

$(NAME): $(OBJS) 
	g++ $(CFLAGS) -o $(NAME) $(OBJS) -lncurses

%.o: %.c
	gcc $(CFLAGS) -c $<

%.o: %.cpp
	g++ $(CFLAGS) -c $<

