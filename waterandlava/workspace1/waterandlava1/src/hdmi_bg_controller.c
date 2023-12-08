#include "hdmi_bg_controller.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sleep.h"

struct coordinates plate_to_door[NUM_PLATES*2];

// background generation test function
void level1_test(){

	//Register write and readback test
	// uint32_t checksum[ROWS*3], readsum[ROWS*3];

	// for (int j = 0; j < ROWS*3; j++)
	// {
	// 	checksum[j] = 0;
	// 	for (int i = 0; i < COLUMNS; i++)
	// 	{
	// 		hdmi_ctrl->VRAM[j*COLUMNS + i] = i + j;
	// 		checksum[j] += i + j;
	// 	}
	// }
	// hdmi_ctrl->Scene = 0x10101010;

	// for (int j = 0; j < ROWS*3; j++)
	// {
	// 	readsum[j] = 0;
	// 	for (int i = 0; i < COLUMNS; i++)
	// 	{
	// 		readsum[j] += hdmi_ctrl->VRAM[j*COLUMNS + i];
	// 		//xil_printf ("%x \n\r", hdmi_ctrl->VRAM[j*COLUMNS + i]);
	// 	}
	// 	xil_printf ("Row: %d, Checksum: %x, Read-back Checksum: %x\n\r", j, checksum[j], readsum[j]);
	// 	if (checksum[j] != readsum[j])
	// 	{
	// 		xil_printf ("Checksum mismatch!, check your SV code\n\r");
	// 		while (1){};
	// 	}
	// }

	// xil_printf ("Checksum code passed!...starting level test \n\r");

	// for( int i=0; i<ROWS*3; i++){
	// 	for(int j=0; j<COLUMNS; j++){
	// 		hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x04;
	// 	}
	// }

	// assign level 1 ram to item indices
	for( int i=0; i<ROWS; i++){
		for(int j=0; j<COLUMNS; j++){
			//top and bottom floors
			if(i<2 || i>27){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x00;
			}
			//left and right walls
			else if(j<4 || j>75){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x01;
			}
			//platform between (15,30) and (15,50)
			else if(i==15 && j>30 && j<50){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x00;
			}
			//platform between (20,2) and (20,30)
			else if(i==20 && j>=2 && j<30){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x00;
			}
			//platform between (5,40) and (5,70)
			else if(i==5 && j>40 && j<70){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x00;
			}
			//platform between (10,20) and (10,30)
			else if(i==10 && j>20 && j<30){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x00;
			}
			//room 1 right wall
			else if(j==69 && i>=2 && i<5){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x01;
			}
			//room 1 left wall
			else if(j==41 && i>=2 && i<3){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x01;
			}
			//puzzle door 1
			else if(j==41 && i>=3 && i<5){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x07;
			}
			//pressure plate 1
			else if(j==40 && i==14){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x06;
			}
			//red door
			else if( (j==55 || j==54) && (i==4 || i==3) ){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x02;
			}
			//blue door
			else if( (j==15 || j==14) && (i==19 || i==18) ){
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x03;
			}
			//background
			else {
				hdmi_ctrl->VRAM[i*COLUMNS + j] = 0x08;
			}
		}
	}

	// assign level 2 ram to item indices
	for( int i2=ROWS; i2<ROWS*2; i2++){
		for(int j=0; j<COLUMNS; j++){
			//top and bottom floors
			int i = i2 - ROWS;
			if(i<2 || i>27){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x00;
			}
			//left and right walls
			else if(j<4 || j>75){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x01;
			}
			//platform between (10,30) and (10,60)
			else if(i==10 && j>30 && j<60){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x00;
			}
			//platform between (20,30) and (20,60)
			else if(i==20 && j>30 && j<60){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x00;
			}
			//platform between (15,70) and (15,75)
			else if(i==15 && j>70 && j<=75){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x00;
			}
			//room 1/2 left wall
			else if(j==31 && i>=2 && i<18){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x01;
			}
			//room 1/2 right wall part 1
			else if(j==49 && i>=2 && i<8){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x01;
			}
			//room 1/2 right wall part 2
			else if(j==49 && i>=11 && i<20){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x01;
			}
			//puzzle door 1
			else if(j==31 && i>=18 && i<20){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x07;
			}
			//puzzle door 2
			else if(j==49 && i>=8 && i<10){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x07;
			}
			//pressure plate 1
			else if(j==60 && i==27){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x06;
			}
			//pressure plate 2
			else if(j==48 && i==19){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x06;
			}
			//red door
			else if( (j==40 || j==39) && (i==19 || i==18) ){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x02;
			}
			//blue door
			else if( (j==40 || j==39) && (i==9 || i==8) ){
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x03;
			}
			//background
			else {
				hdmi_ctrl->VRAM[i2*COLUMNS + j] = 0x08;
			}
		}
	}


	// assign level 3 ram to item indices
	for( int i3=ROWS*2; i3<ROWS*3; i3++){
		for(int j=0; j<COLUMNS; j++){
			int i = i3 - (ROWS*2);
			//top and bottom floors
			if(i<2 || i>27){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x00;
			}
			//left and right walls
			else if(j<4 || j>75){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x01;
			}
			//platform between (10,4) and (10,60)
			else if(i==10 && j>=4 && j<60){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x00;
			}
			//platform between (20,20) and (20,75)
			else if(i==20 && j>20 && j<=75){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x00;
			}
			//platform between (19,41) and (19,43)
			else if(i==19 && j>=41 && j<43){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x00;
			}
			//lava between (19,43) and (19,55)
			else if(i==19 && j>=43 && j<55){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x04;
			}
			//platform between (19,55) and (19,60)
			else if(i==19 && j>=55 && j<60){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x00;
			}
			//water between (19,60) and (19,76)
			else if(i==19 && j>=60 && j<76){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x05;
			}
			//platform between (27,41) and (27,43)
			else if(i==27 && j>=41 && j<43){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x00;
			}
			//lava between (27,43) and (27,55)
			else if(i==27 && j>=43 && j<55){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x04;
			}
			//platform between (27,55) and (27,60)
			else if(i==27 && j>=55 && j<60){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x00;
			}
			//water between (27,60) and (27,76)
			else if(i==27 && j>=60 && j<76){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x05;
			}
			//room 1 left wall
			else if(j==21 && i>20 && i<26){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x01;
			}
			//room 2 right wall
			else if(j==21 && i>=2 && i<8){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x01;
			}
			//puzzle door 1
			else if(j==21 && i>=25 && i<=27){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x07;
			}
			//puzzle door 2
			else if(j==21 && i>=8 && i<10){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x07;
			}
			//pressure plate 1
			else if(j==57 && i==18){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x06;
			}
			//pressure plate 2
			else if(j==57 && i==26){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x06;
			}
			//red door
			else if( (j==30 || j==29) && (i==27 || i==26) ){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x02;
			}
			//blue door
			else if( (j==30 || j==29) && (i==9 || i==8) ){
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x03;
			}
			//background
			else {
				hdmi_ctrl->VRAM[i3*COLUMNS + j] = 0x08;
			}
		}
	}

	// (active) (sprite index) (x coordinate) (y coordinate)
	for(int i=0; i<10; i++){
		hdmi_ctrl->Sprite[i] = (1 << 31) | ((i%6) << 24) | ((i*18) << 12) | (i*18);
	}

//	int x,y;
//	x1=0; y1=0;
//	int scount=0;
//	while(1){
//		hdmi_ctrl->Scene = scount%3;
//		scount++;
//		hdmi_ctrl->Sprite[0] = (hdmi_ctrl->Sprite[0] & (255 << 24)) | (x << 12) | (y);
//		x = (x+1)%640;
//		y = (y+1)%480;
//		sleep_MB(5);
//	}
}

// assigns walls and background tiles to VRAM
// param:
// int scene - the scene to be drawn to
void generate_background(int scene){
	for( int i=0; i<ROWS; i++){
		for(int j=0; j<COLUMNS; j++){
			//ceiling and floor
			if( i<2 || i>27 ){
				hdmi_ctrl->VRAM[(i + (ROWS*scene))*COLUMNS + j] = FLOOR;
			}
			//left and right walls
			else if( j<4 || j>75 ){
				hdmi_ctrl->VRAM[(i + (ROWS*scene))*COLUMNS + j] = WALL;
			}
			//background
			else {
				hdmi_ctrl->VRAM[(i + (ROWS*scene))*COLUMNS + j] = BACKGROUND;
			}
		}
	}
}

// creates a platform at a given location
// param:
// int scene - the scene to be drawn to
// int start_x - tile start point of the platform
// int end_x - tile end point of the platform
// int y - the tile y location of the platform
void make_platform(int scene, int start_x, int end_x, int y){
	for(int j=start_x; j<=end_x; j++){
		hdmi_ctrl->VRAM[ (y + (ROWS*scene))*COLUMNS + j] = FLOOR;
	}
}

// creates lava at a given location
// param:
// int scene - the scene to be drawn to
// int start_x - tile start point of the lava
// int end_x - tile end point of the lava
// int y - the tile y location of the lava
void make_lava(int scene, int start_x, int end_x, int y){
	for(int j=start_x; j<=end_x; j++){
		hdmi_ctrl->VRAM[ (y + (ROWS*scene))*COLUMNS + j] = LAVA;
	}
}

// creates water at a given location
// param:
// int scene - the scene to be drawn to
// int start_x - tile start point of the water
// int end_x - tile end point of the water
// int y - the tile y location of the water
void make_water(int scene, int start_x, int end_x, int y){
	for(int j=start_x; j<=end_x; j++){
		hdmi_ctrl->VRAM[ (y + (ROWS*scene))*COLUMNS + j] = WATER;
	}
}

// creates a wall at a given location
// param:
// int scene - the scene to be drawn to
// int x - the tile x location of the wall
// int start_y - start point of the wall
// int end_y - end point of the wall
void make_wall(int scene, int x, int start_y, int end_y){
	for(int i=start_y; i<=end_y; i++){
		hdmi_ctrl->VRAM[(i + (ROWS*scene))*COLUMNS + x] = WALL;
	}
}

// creates a pressure plate at a given location
// param:
// int scene - the scene to be drawn to
// int x - the tile x location of the pressure plate
// int y - the tile y location of the pressure plate
void make_pressureplate(int scene, int x, int y){
	hdmi_ctrl->VRAM[(y + (ROWS*scene))*COLUMNS + x] = PRESSURE;
}

// creates a puzzle door at a given location
// param:
// int scene - the scene to be drawn to
// int x - the tile x location of the door
// int y - the top tile y location of the door
void make_puzzledoor(int scene, int x, int y){
	for(int i=y; i<y+2; i++){
		hdmi_ctrl->VRAM[(i + (ROWS*scene))*COLUMNS + x] = PUZZLE_DOOR;
	}
}

// removes a puzzle door at a given location
// param:
// int scene - the scene to be drawn to
// int x - the tile x location of the door
// int y - the top tile y location of the door
void remove_puzzledoor(int scene, int x, int y){
	for(int i=y; i<y+2; i++){
		hdmi_ctrl->VRAM[(i + (ROWS*scene))*COLUMNS + x] = BACKGROUND;
	}
}

// creates an exit door at a given location
// param:
// int scene - the scene to be drawn to
// int color - 1 means blue door, 0 means red door
// int x - the tile x location of the door
// int y - the tile y location of the door
void make_exitdoor(int scene, int color, int x, int y){
	for( int i=y + ROWS*scene; i<y + ROWS*scene + 2; i++){
		for(int j=x; j<x+2; j++){
			if( color == 1 )
				hdmi_ctrl->VRAM[i*COLUMNS + j] = BLUE_DOOR;
			else
				hdmi_ctrl->VRAM[i*COLUMNS + j] = RED_DOOR;
		}
	}
}

// creates a star at a given location
// param:
// int scene - the scene to be drawn to
// int x - the tile x location of the star
// int y - the tile y location of the star
make_star(int scene, int x, int y){
	hdmi_ctrl->VRAM[(y + (ROWS*scene))*COLUMNS + x] = STAR;
}

// moves a the star at x,y to the collected location given 
// the number of collected stars
// param:
// int scene - the scene to be drawn to
// int star_count - the number of stars already collected
// int x - the tile x location of the star
// int y - the tile y location of the star
void collect_star(int scene, int star_count, int x, int y){
	//removes exisiting star
	hdmi_ctrl->VRAM[((int)(y/16) + ROWS*scene)*COLUMNS + (int)(x/8)] = BACKGROUND;

	//adds to scorekeeping
	hdmi_ctrl->VRAM[(ROWS*scene)*COLUMNS + star_count] = COLLECTED_STAR;
}

// gets the value of a tile given an x and y value
// param:
// int scene - the scene to be drawn to
// int x - requested x value
// int y - requested y value
// return:
// the value stored in the tile that contains the pixel (x,y)
uint32_t get_tile(int scene, int x, int y){
	return hdmi_ctrl->VRAM[((int)(y/16) + ROWS*scene)*COLUMNS + (int)(x/8)];
}

// assigns the scene being displayed
// param:
// int scene - assigned scene
void set_scene(int scene){
	hdmi_ctrl->Scene = scene;
}

// assigns a coordinate to a given sprite
// param:
// int sprite - the sprite being moved
// int x - the x value of the sprite
// int y - the y value of the sprite
void set_sprite_coord(int sprite, int x, int y){
	x = x & 0xfff;
	y = y & 0xfff;
	hdmi_ctrl->Sprite[sprite] = (hdmi_ctrl->Sprite[sprite] & (255 << 24)) | (x << 12) | (y);
}

// assigns an index to a given sprite
// param:
// int sprite - the sprite being moved
// int index - the desrired sprite index
void set_sprite_index(int sprite, int index){
	index = index & 127;
	hdmi_ctrl->Sprite[sprite] = (hdmi_ctrl->Sprite[sprite] & ~(127 << 24)) | (index << 24);
}

// assigns an active state to a given sprite
// param:
// int sprite - the sprite being changed 
// int active - the state of the sprite, 0 means inactive, 1 means active
void set_sprite_active(int sprite, int active){
	active = active & 1;
	hdmi_ctrl->Sprite[sprite] = (hdmi_ctrl->Sprite[sprite] & ~(1 << 31)) | (active << 31);
}

// generates the first level to a given scene
// param:
// int scene - scene to be drawn to
void generate_level1(int scene){
	generate_background(scene);

	make_platform(scene, 30, 50, 15);

	make_platform(scene, 2, 30, 20);

	make_platform(scene, 38, 70, 7);

	make_platform(scene, 20, 30, 10);

	make_platform(scene, 30, 60, 25);

	make_wall(scene, 70, 2, 7);

	make_wall(scene, 41, 2, 5);

	make_puzzledoor(scene, 41, 5);

	make_pressureplate(scene, 40, 14);

	make_exitdoor(scene, 0, 54, 5);

	make_exitdoor(scene, 1, 14, 18);

	make_star(scene, 43, 14);

	make_star(scene, 56, 6);

	make_star(scene, 25, 27);

	//clears exisiting data
	for(int i=0; i<NUM_PLATES*2; i++){
		plate_to_door[i].active = FALSE;
		plate_to_door[i].x = 0;
		plate_to_door[i].y = 0;
	}
	//pressure plate 1 coordinates
	plate_to_door[0].x = 40;
	plate_to_door[0].y = 14;
	//puzzle door 1 coordinates
	plate_to_door[1].x = 41;
	plate_to_door[1].y = 5;
}

// generates the second level to a given scene
// param:
// int scene - scene to be drawn to
void generate_level2(scene){
	generate_background(scene);

	make_platform(scene, 28, 59, 10);

	make_platform(scene, 70, 75, 15);

	make_star(scene, 72, 14);

	make_platform(scene, 70, 75, 25);

	make_platform(scene, 28, 59, 20);

	make_platform(scene, 20, 25, 22);

	make_star(scene, 23, 21);

	make_platform(scene, 20, 25, 15);

	make_star(scene, 23, 14);

	make_wall(scene, 31, 2, 17);

	make_wall(scene, 49, 2, 7);

	make_wall(scene, 49, 11, 19);
	
	make_puzzledoor(scene, 31, 18);

	make_puzzledoor(scene, 49, 8);

	make_pressureplate(scene, 60, 27);

	make_pressureplate(scene, 45, 19);

	make_exitdoor(scene, 0, 39, 18);

	make_exitdoor(scene, 1, 39, 8);

	//clears exisiting data
	for(int i=0; i<NUM_PLATES*2; i++){
		plate_to_door[i].active = FALSE;
		plate_to_door[i].x = 0;
		plate_to_door[i].y = 0;
	}
	//pressure plate 1 coordinates
	plate_to_door[0].x = 60;
	plate_to_door[0].y = 27;
	//puzzle door 1 coordinates
	plate_to_door[1].x = 31;
	plate_to_door[1].y = 18;
	//pressure plate 2 coordinates
	plate_to_door[2].x = 45;
	plate_to_door[2].y = 19;
	//puzzle door 2 coordinates
	plate_to_door[3].x = 49;
	plate_to_door[3].y = 8;

}

// generates the third level to a given scene
// param:
// int scene - scene to be drawn to
void generate_level3(scene){
	generate_background(scene);

	make_platform(scene, 4, 59, 10);

	make_platform(scene, 70, 75, 15);

	make_star(scene, 72, 14);

	make_platform(scene, 21, 75, 20);

	make_star(scene, 30, 19);

	make_platform(scene, 41, 42, 19);

	make_platform(scene, 3, 8, 22);

	make_star(scene, 6, 21);

	make_lava(scene, 43, 54, 19);

	make_platform(scene, 55, 59, 19);

	make_water(scene, 60, 75, 19);

	make_platform(scene, 41, 42, 27);

	make_lava(scene, 43, 54, 27);

	make_platform(scene, 55, 59, 27);

	make_water(scene, 60, 75, 27);

	make_wall(scene, 21, 20, 25);

	make_wall(scene, 21, 2, 7);

	make_puzzledoor(scene, 21, 26);

	make_puzzledoor(scene, 21, 8);

	make_pressureplate(scene, 57, 18);

	make_pressureplate(scene, 57, 26);

	make_exitdoor(scene, 0, 29, 26);

	make_exitdoor(scene, 1, 10, 8);

	//clears exisiting data
	for(int i=0; i<NUM_PLATES*2; i++){
		plate_to_door[i].active = FALSE;
		plate_to_door[i].x = 0;
		plate_to_door[i].y = 0;
	}
	//pressure plate 1 coordinates
	plate_to_door[0].x = 57;
	plate_to_door[0].y = 18;
	//puzzle door 1 coordinates
	plate_to_door[1].x = 21;
	plate_to_door[1].y = 26;
	//pressure plate 2 coordinates
	plate_to_door[2].x = 57;
	plate_to_door[2].y = 26;
	//puzzle door 2 coordinates
	plate_to_door[3].x = 21;
	plate_to_door[3].y = 8;

}

void draw_vert(int scene, int x, int start_y, int end_y, int value ){
	for( int i=start_y; i<=end_y; i++){
		hdmi_ctrl->VRAM[ (i + (ROWS*scene))*COLUMNS + x] = value;
	}
}
void draw_horz(int scene, int start_x, int end_x, int y, int value ){
	for( int i=start_x; i<=end_x; i++){
		hdmi_ctrl->VRAM[ (y + (ROWS*scene))*COLUMNS + i] = value;
	}
}

// generates the menu screen on a given scene
// param:
// int scene - scene to be drawn to
void generate_menu(int scene){
	for( int i=0; i<ROWS; i++ ){
		draw_horz(scene, 0, COLUMNS-1, i, BACKGROUND);
	}

	//W
	draw_vert(scene, 4, 4, 7, WATER);
	draw_vert(scene, 5, 8, 10, WATER);
	draw_vert(scene, 6, 11, 11, WATER);
	draw_vert(scene, 7, 8, 10, WATER);
	draw_vert(scene, 8, 8, 8, WATER);
	draw_vert(scene, 12, 4, 7, WATER);
	draw_vert(scene, 11, 8, 10, WATER);
	draw_vert(scene, 10, 11, 11, WATER);
	draw_vert(scene, 9, 8, 10, WATER);

	//A
	draw_vert(scene, 13, 10, 11, WATER);
	draw_vert(scene, 14, 7, 9, WATER);
	draw_vert(scene, 15, 5, 6, WATER);
	draw_vert(scene, 16, 4, 4, WATER);
	draw_vert(scene, 19, 10, 11, WATER);
	draw_vert(scene, 18, 7, 9, WATER);
	draw_vert(scene, 17, 5, 6, WATER);
	make_water(scene, 15, 17, 8);

	//T
	make_water(scene, 20, 26, 4);
	draw_vert(scene, 23, 5, 11, WATER);

	//E 
	make_water(scene, 28, 32, 4);
	make_water(scene, 28, 32, 8);
	make_water(scene, 28, 32, 11);
	draw_vert(scene, 28, 4, 11, WATER);

	//R
	make_water(scene, 34, 37, 4);
	make_water(scene, 34, 37, 7);
	make_water(scene, 35, 35, 8);
	make_water(scene, 36, 36, 9);
	make_water(scene, 37, 37, 10);
	make_water(scene, 38, 38, 11);
	draw_vert(scene, 34, 5, 11, WATER);
	draw_vert(scene, 38, 5, 6, WATER);

	//+
	draw_vert(scene, 44, 6, 10, 0x7f);
	draw_vert(scene, 45, 6, 10, 0x7f);
	draw_horz(scene, 41, 48, 8, 0x7f);

	//L
	draw_vert(scene, 51, 4, 11, LAVA);
	draw_horz(scene, 52, 53, 11, LAVA);

	//A
	draw_vert(scene, 55, 10, 11, LAVA);
	draw_vert(scene, 56, 7, 9, LAVA);
	draw_vert(scene, 57, 5, 6, LAVA);
	draw_vert(scene, 58, 4, 4, LAVA);
	draw_vert(scene, 59, 5, 6, LAVA);
	draw_vert(scene, 60, 7, 9, LAVA);
	draw_vert(scene, 61, 10, 11, LAVA);
	make_lava(scene, 57, 59, 8);

	//V 
	draw_vert(scene, 62, 4, 5, LAVA);
	draw_vert(scene, 63, 6, 7, LAVA);
	draw_vert(scene, 64, 8, 9, LAVA);
	draw_vert(scene, 65, 10, 11, LAVA);
	draw_vert(scene, 66, 8, 9, LAVA);
	draw_vert(scene, 67, 6, 7, LAVA);
	draw_vert(scene, 68, 4, 5, LAVA);

	//A
	draw_vert(scene, 69, 10, 11, LAVA);
	draw_vert(scene, 70, 7, 9, LAVA);
	draw_vert(scene, 71, 5, 6, LAVA);
	draw_vert(scene, 72, 4, 4, LAVA);
	draw_vert(scene, 73, 5, 6, LAVA);
	draw_vert(scene, 74, 7, 9, LAVA);
	draw_vert(scene, 75, 10, 11, LAVA);
	make_lava(scene, 71, 73, 8);

	//1
	draw_vert(scene, 38, 19, 23, 0x7f);
	draw_horz(scene, 37, 39, 23, 0x7f);
	draw_horz(scene, 37, 37, 20, 0x7f);

	//2 
	draw_horz(scene, 43, 45, 19, 0x7f);
	draw_horz(scene, 43, 43, 20, 0x7f);
	draw_vert(scene, 45, 19, 21, 0x7f);
	draw_horz(scene, 44, 44, 22, 0x7f);
	draw_horz(scene, 43, 45, 23, 0x7f);

	//3
	draw_horz(scene, 49, 51, 19, 0x7f);
	draw_horz(scene, 49, 51, 21, 0x7f);
	draw_horz(scene, 49, 51, 23, 0x7f);
	draw_vert(scene, 51, 19, 23, 0x7f);

}

// creates a lose screen
// param:
// int scene - scene to be drawn to
void generate_lose(int scene){

	for( int i=10; i<=20; i++ ){
		draw_horz(scene, 30, 39, i, FLOOR);
	}
	for( int i=10; i<=20; i++ ){
		draw_horz(scene, 40, 50, i, FLOOR);
	}

	draw_horz(scene, 35, 45, 19, LAVA);
	draw_vert(scene, 35, 11, 19, LAVA);
}

// creates a win screen
// param:
// int scene - scene to be drawn to
void generate_win(int scene){

	for( int i=10; i<=20; i++ ){
		draw_horz(scene, 30, 39, i, FLOOR);
	}
	for( int i=10; i<=20; i++ ){
		draw_horz(scene, 40, 50, i, FLOOR);
	}

	//W
	draw_vert(scene, 36, 12, 15, WATER);
	draw_vert(scene, 37, 16, 18, WATER);
	draw_vert(scene, 38, 19, 19, WATER);
	draw_vert(scene, 39, 17, 18, WATER);
	draw_vert(scene, 40, 19, 19, WATER);
	draw_vert(scene, 41, 17, 18, WATER);
	draw_vert(scene, 42, 19, 19, WATER);
	draw_vert(scene, 43, 16, 18, WATER);
	draw_vert(scene, 44, 12, 15, WATER);
}