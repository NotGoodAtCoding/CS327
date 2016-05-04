CFLAGS = -g -Wall -Werror 

OBJS = main.o \
character.o \
core.o \
debug.o \
dice.o \
dijkstra.o \
dungeon.o \
equipment.o \
heap.o \
inventory.o \
item.o \
monsterfactory.o \
move.o \
npc.o \
objectfactory.o \
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

%.o: %.cpp
	g++ $(CFLAGS) -c $<

