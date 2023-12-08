#include "lw_usb/GenericTypeDefs.h"
#include "xstatus.h"
#include "xparameters.h"

#define COLUMNS 80
#define ROWS 30
#define PALETTE_START 0x2000

#define WALL 0x00
#define FLOOR 0x01
#define RED_DOOR 0x02
#define BLUE_DOOR 0x03
#define LAVA 0x04
#define WATER 0x05
#define PRESSURE 0x06
#define PUZZLE_DOOR 0x07
#define BACKGROUND 0x08
#define STAR 0x09
#define COLLECTED_STAR 0x0a

#define BLACK 0x7f

#define FIREBOY_STAND 0x00
#define FIREBOY_RIGHT 0x01
#define FIREBOY_LEFT 0x02
#define WATERGIRL_STAND 0x03
#define WATERGIRL_RIGHT 0x04
#define WATERGIRL_LEFT 0x05

#define SPRITE_SIZE_X 16
#define SPRITE_SIZE_Y 32

#define SPRITE_SPEED_X 4
#define SPRTIE_SPEED_Y 16

#define NUM_PLATES 10

struct BG_HDMI_STRUCT {
	uint8_t		      	VRAM [ROWS*COLUMNS*3]; // VRAM/scene data
	uint32_t			Scene; // scene select
	uint32_t 			UNUSED [7];
	uint32_t			Sprite [10]; // sprite info registers
};

// creates an HDMI data structure starting at BRAM location 0x000
static volatile struct BG_HDMI_STRUCT* hdmi_ctrl = XPAR_HDMI_TEXT_CONTROLLER_WAL_AXI_BASEADDR;

//coordinate struct
struct coordinates {
    int x;
    int y;
	BOOL active;
};

//pressure plate info
//plate_to_door[2k] : for even i's in 2k, [i] represents the coorinates of plate, [i+1] represents the coordinates of the door
extern struct coordinates plate_to_door[NUM_PLATES*2];

void level1_test();
void generate_background(int scene);
void make_platform(int scene, int start_x, int end_x, int y);
void make_wall(int scene, int x, int start_y, int end_y);
void make_pressureplate(int scene, int x, int y);
void make_puzzledoor(int scene, int x, int y);
void remove_puzzledoor(int scene, int x, int y);
void make_exitdoor(int scene, int color, int x, int y);
void collect_star(int scene, int star_count, int x, int y);
uint32_t get_tile(int scene, int x, int y);
void set_scene(int scene);
void set_sprite_coord(int sprite, int x, int y);
void set_sprite_index(int sprite, int index);
void set_sprite_active(int sprite, int active);
void generate_level1(int scene);
void generate_level2(int scene);
void generate_level3(int scene);
void generate_menu(int scene);
void generate_win(int scene);
void generate_lose(int scene);
