#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

char dungeon[80][21];
int room_centers[7][2];

int init_dungeon(int rooms);

int check_range(int x_min, int x_max, int y_min, int y_max){
	int i, j;
	for(i=x_min-1; i<=x_max; i++){
		for(j = y_min-1; j<=y_max; j++){
			if(dungeon[i][j] == '.'){
				return 1;
			}
		}
	}
	return 0;
}

int draw_path(int x, int y){
	if(dungeon[x][y] == '.'){
		return 1;
	}
	else{
		dungeon[x][y] = '#';
	}
	return 0;
}

int y_path(int x, int y, int steps){
	int i;
	if(steps < 0){
		for(i=0 ; i > steps; i--){
			draw_path(x, y+i);
		} 
	}
	else{
		for(i=0 ; i< steps; i++){
			draw_path(x, y+i);
		}
	}
	return 0;
}

int x_path(int x, int y, int steps){
	int i;
	if(steps < 0){
		for(i=0 ; i > steps; i--){
			draw_path(x+i, y);
		} 
	}
	else{
		for(i=0 ; i< steps; i++){
			draw_path(x+i, y);
		}
	}
	return 0;
}

int make_path(int room0, int room1){
	int x_mark = room_centers[room0][0];
	int y_mark = room_centers[room0][1];
	int x_steps = (room_centers[room1][0] - room_centers[room0][0]);
	int y_steps = (room_centers[room1][1] - room_centers[room0][1]);
	
	x_path(x_mark, y_mark, x_steps);
	x_mark += x_steps;
	y_path(x_mark, y_mark, y_steps);
	return 0;
}

int gen_paths(int rooms){
	int i;
	for(i =1; i < rooms; i++){
		make_path(i-1, i);
	}
	return 0;
}

int make_room(int x, int y, int width, int height){
	if((x + width) >79 || (y + height) >20){
		return 1;
	}
	if(check_range(x,(x+width), y, (y+height)) == 1){
		return 1;
	}
	
	int i, j =0;
	for(i=0; i<width; i++){
		for(j=0; j<height; j++){
			dungeon[x+i][y+j] = '.';
		}
	}
	return 0;
}

int gen_rooms(int rooms){
	int i, fails=0;
	for(i=0;i<rooms;i++){
		if(fails >100){
			init_dungeon(rooms);
			break;
		}
		int r = rand();
		int x = (r % 77) +2;
		int y = (r % 18) +2;
		int width = (r% 40) +6;
		int height = (r % 18) +4;
		if(make_room(x, y, width, height) ==1){
			i--;
			fails++;
			continue;
		}
		else {
			room_centers[i][0] = x + (width / 2);
			room_centers[i][1] = y + (height / 2);
		}
	}
	return 0;
}

int init_dungeon(int rooms){
	int i, j;
	for(i=0; i<21; i++){
		for(j=0; j<80; j++){
			dungeon[j][i] = ' ';
		}
	}
	gen_rooms(rooms);
	gen_paths(rooms);
	return 0;
}

void render(){
	int i, j;
	for(i=0; i<21; i++){
		for(j=0; j<80; j++){
			printf("%c", dungeon[j][i]);
		}
		printf("\n");
	}
	return;
}

int main(int argc, char *argv[]){
	if(argc !=2) {
		printf("Improper Use, consult README\n");
		return 1;
	}
	int rooms = atoi(argv[1]);
	if(rooms > 7) {
		printf("Too many rooms, consult README\n");
		return 1;
	}
	srand(time(NULL));
	init_dungeon(rooms);
	printf("Random: %d\n", rand());
	render();
	return 0;
}
