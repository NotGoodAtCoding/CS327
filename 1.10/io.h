#ifndef IO_H
# define IO_H

#define COLOR_HP   17
#define COLOR_MP   27
#define COLOR_GOLD 17

typedef struct dungeon dungeon_t;

void io_init_terminal(dungeon_t *d);
void io_reset_terminal(void);
void io_display(dungeon_t *d);
void io_handle_input(dungeon_t *d);
void io_queue_message(const char *format, ...);
void io_print_hud(dungeon_t *d);


#endif
