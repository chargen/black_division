#include "game.h"
#include "devices.h"
#include "mouse.h"
#include "rtc.h"
#include "keyboard.h"
#include "vbe.h"
#include "video_gr.h"
#include "timer.h"
#include "i8042.h"
#include "i8254.h"

/* constants */
const unsigned int TIMER_IRQ_SET = 1;	/**< @brief Timer IRQ SET */
const unsigned int KEYBOARD_IRQ_SET = 2;	/**< @brief Keyboard ID */
const unsigned int MOUSE_IRQ_SET = 4;	/**< @brief Mouse ID */

const int HMAX = 1024;	/**< @brief Horizontal resolution. */
const int VMAX = 768;	/**< @brief Vertical resolution. */

const char str_end = '.';	/**< @brief Char used for "end of string". */
const char str_newline = ':';	/**< @brief Char used for "new line".*/

unsigned short int debug = 0;	/**< @brief Variable containing the information about debugmode (0 = OFF ; 1 = ON). */
unsigned int lvl = 1;			/**< @brief Game level. */
unsigned int difficulty = 1;	/**< @brief Game difficulty. */
unsigned int next_level_score;	/**< @brief Score for the next level. */

unsigned int player_colors[] = { 0, 32, 17};		/**< @brief Colors of the player sprite. */
unsigned int colors[] = { 63, 36, 18, 27, 54, 38, 53};		/**< @brief General colors of the game. */
unsigned int colors_faded[] = { 0, 4, 2, 57, 55, 20, 21};		/**< @brief General faded colors of the game. */
unsigned int colors_takeaway[] = { 100, 90, 20, 70, 50, 40, 30};	/**< @brief Amount of power to take from the player - related to each color of colors[]. */
const int colors_size = 7;	/**< @brief Size of the colors arrays. */

unsigned int safe_color = 0;	/**< @brief Index of the current "safe color". */
unsigned int safe_holes = 6;	/**< @brief Number of holes containing the "safe color". */
unsigned int black_holes = 0;	/**< @brief Number of holes containing the black color. */
unsigned int divisions = 8;		/**< @brief Number of board divisions. */
int hole_color[8];	/**< @brief Array containing the color of the 8 board divisions. */

unsigned int red_square_size = 4;	/**< @brief Size of the moving red square. */
unsigned int red_square_index = 1;	/**< @brief Current index of the red quare. */
unsigned int red_square_color = 4;	/**< @brief Color of the red square. */
unsigned int red_square_speed = 2;	/**< @brief Speed of the red square. */

short int first_round = 1;	/**< @brief Variable containing the information on whether it is or not the first round. */
const int reasonable_iterations = 100;

unsigned int option = 0;	/**< @brief Current menu option. This options are enumerated in the "core options indexes" enum. */
unsigned int back_color = 57;	/**< @brief Background color for the menus. */

/* player direction */
/** @name  player direction */
/**@{
 *
 * Translates player directions into numerical values
 */
enum {
	STALL,
	UP,
	RIGHT,
	DOWN,
	LEFT
};
/** @} end of player direction */

/** @name  colors indexes */
/**@{
 *
 * Simbolic names for the numbers in colors[]
 */
enum {
	WHITE,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	ORANGE,
	PURPLE
};
/** @} end of colors indexes */

/** @name  options indexes */
/**@{
 *
 * Simbolic names for the menu options
 */
enum {
	MENU,
	MENU_HELP,
	MENU_OPTIONS,
	MENU_CREDITS,
	GAME
};
/** @} end of options indexes */

/** @name  titles indexes */
/**@{
 *
 * Simbolic names for the menu titles
 */
enum {
	TITLE,
	PLAY,
	HELP,
	OPTIONS,
	CREDITS,
	QUIT
};
/** @} end of titles indexes */

/** @name  bar indexes */
/**@{
 *
 * Simbolic names for the game bars
 */
enum {
	LIVES,
	ENERGY,
	SAFE_COLOR,
	SCORE,
	LEVEL,
	NEXT_LVL
};
/** @} end of bar indexes */

/** @name  cursor struct */
/**@{
 *
 * Contains all the cursor info
 */
struct {
	char sprite[5][5];	/**< @brief Cursor sprite, initialized below */
	unsigned int width;	/**< @brief Cursor width */
	unsigned int size;	/**< @brief Cursor size*/
}cursor
/* cursor sprite */
= {
		{		{"7777 "},
				{"77   "},
				{"7 7  "},
				{"7  7 "},
				{"    7"}}
};
/** @} end of cursor struct */

/** @name  mouses struct */
/**@{
 *
 * Contains all the info of the mouse devices
 */
typedef struct {
	unsigned char packet[3];	/**< @brief Mouse packets */
	unsigned int packetCounter; /**< @brief Mouse packet counter */
	int x;	/**< @brief Mouse relative x position */
	int y;	/**< @brief Mouse relative y position */
	int buttonPressed;	/**< @brief Mouse pressed buttons */
}MOUSES;
/** @} end of mouses struct */

/** @name  titles struct */
/**@{
 *
 * Contains all the info on the menu titles
 */
typedef struct {
	char * title;	/**< @brief Pointer to the title string */
	int visible;	/**< @brief If the title is or not visible */
	int x;	/**< @brief x top-left coordinate */
	int y;	/**< @brief y top-left coordinate */
	unsigned int hSize;	/**< @brief Horizontal size */
	unsigned int vSize;	/**< @brief Vertical size */
	unsigned int color;	/**< @brief Title color */
}TITLES;
/** @} end of titles struct */

/** @name  bars struct */
/**@{
 *
 * Contains all the info on the game bars/titles
 */
typedef struct {
	char * title;	/**< @brief Pointer to the bar string */
	int visible;	/**< @brief If the bar is or not visible */
	int x;	/**< @brief x top-left coordinate */
	int y;	/**< @brief y top-left coordinate */
	unsigned int hSize;	/**< @brief Horizontal size */
	unsigned int vSize;	/**< @brief Vertical size */
	unsigned int color;	/**< @brief Bar color */
}BARS;
/** @} end of bars struct */

/** @name  timers struct */
/**@{
 *
 * Contains all the info of the mouse devices
 */
typedef struct {
	unsigned int counter; 	/**< @brief Timer counter value */
}TIMERS;
/** @} end of timers struct */

/** @name  scenarios struct */
/**@{
 *
 * Contains all the info on the board scenarios
 */
typedef struct {
	int x;	/**< @brief x top-left coordinate */
	int y;	/**< @brief y top-left coordinate */
	unsigned int size;	/**< @brief scenario size */
	unsigned int color;	/**< @brief scenario color */
}SCENARIOS;
/** @} end of scenarios struct */

/** @name  keyboards struct */
/**@{
 *
 * Contains all the info on the keyboard devices
 */
typedef struct {
	unsigned int code; /**< @brief keyboard received code */
}KEYBOARDS;
/** @} end of keyboards struct */

/** @name  dates struct */
/**@{
 *
 * Contains all time info
 */
typedef struct {
	int hours;	 /**< @brief Hour */
	int minutes;	 /**< @brief Minutes */
	int seconds;	 /**< @brief Seconds */
	long int totalSeconds;	 /**< @brief Total seconds */
}DATES;
/** @} end of dates struct */

/** @name  player struct */
/**@{
 *
 * Contains all the player info
 */
struct {
	char sprite[21][21];	 /**< @brief Player sprite */
	long score;	 /**< @brief Player score */
	int x;	 /**< @brief Player x top-left coordinate */
	int y;	 /**< @brief Player y top-left coordinate */
	int xf;	 /**< @brief Player x down-right coordinate */
	int yf;	 /**< @brief Player y down-right coordinate */
	int width;	 /**< @brief Player width */
	int height;	 /**< @brief Player height */
	int size;	 /**< @brief Player relative size */
	int lives;	 /**< @brief Number of lives */
	int power;	 /**< @brief Player power */
	int xSpeed;	 /**< @brief Player speed on the x axis */
	int ySpeed;	 /**< @brief Player speed on the y axis  */
	int direction;	 /**< @brief Player current moving direction */
}player
/* player sprite */
/* the numbers are indexes to the color in the array player_colors[] */
= {
		{		{"        2   2        "},
				{"       212 212       "},
				{"      212   212      "},
				{"     212     212     "},
				{"    212       212    "},
				{"   212         212   "},
				{"  212     1     212  "},
				{" 212     111     212 "},
				{"212     11211     212"},
				{" 2     112 211     2 "},
				{"      112   211      "},
				{" 2     112 211     2 "},
				{"212     11211     212"},
				{" 212     111     212 "},
				{"  212     1     212  "},
				{"   212         212   "},
				{"    212       212    "},
				{"     212     212     "},
				{"      212   212      "},
				{"       212 212       "},
				{"        2   2        "}}
};
/** @} end of player struct */

/* struct instances definition */
TIMERS timer;
KEYBOARDS keyboard;
MOUSES mouse;
SCENARIOS scenario[SCENARIOS_NUM];
TITLES titles[10];
BARS bars[10];
DATES startDate, endDate;
RTC game_rtc;

//********************************************* SCREEN *******************************
int debugTestWrite(int x, int y, int number) {
	vg_fill_section(x,y,x + 120, y + 40,0);
	if(drawNumber(x,y,number,4,0) == -1) {
		return -1;
	}
	return 0;
}

int drawString(int x, int y, char * str, int size, unsigned int color) {

	unsigned int index = 0;
	int xi = x;
	int returnValue;
	char toCompare;
	toCompare = str[index];

	if(!size) {
		size = CHAR_SIZE;
	}

	while(toCompare != str_end) {

		if(toCompare != str_newline) {
			returnValue = vg_draw_char(x, y, (color != 0) ? color : CHAR_COLOR, toCompare, size);
			if(returnValue == -1) {
				return -1;
			}
			else {
				x += returnValue;
			}
			x += 2*size;
		}
		else {
			y += size*6;
			x = xi;
		}

		index++;
		toCompare = str[index];

	}
	return (x - xi);
}

int drawNumber(int x, int y, long int number, unsigned int color, int size) {

	int length = 0;
	long int number_test = number;

	if (size == 0) {
		size = CHAR_SIZE;
	}

	do {
		number_test = number_test / 10;
		length++;
	} while(number_test != 0);

	length--;

	// draw the " - "
	if(number < 0) {
		if(vg_draw_line(x,y+(3*size/4),x+size,y+(size/2),color) == -1) {
			return -1;
		}
		if(vg_draw_line(x,y+(3*size/4) + 1,x+size,y+(size/2) + 1,color) == -1) {
			return -1;
		}
		number = -number;
	}

	x += (size + (3*size/2)) * length + size;

	do {
		if(vg_draw_number(x, y, color, (number % 10), size) == -1) {
			return -1;
		}
		x -= 2*size;
		number = (number / 10);
	} while(number != 0);

	return ((3*size) * length + size);
}

int clearScreen(unsigned int x, unsigned int y, unsigned int xf, unsigned int yf, unsigned int color) {
	vg_fill(color);
	return 0;
}

int drawError(char * str) {
	clearBuffer(0);
	if(drawString(MENU_X,MENU_Y,str,0,0) ==	-1) {
		return -1;
	}
	drawBufferToScreen();
	return 0;
}

//********************************************* DATE/TIME ****************************
void getStartTime() {
	startDate.hours = rtc_get_hours(game_rtc);
	startDate.minutes = rtc_get_minutes(game_rtc);
	startDate.seconds = rtc_get_seconds(game_rtc);
	startDate.totalSeconds = startDate.hours*3600 + startDate.minutes*60 + startDate.seconds;
}

void getEndTime() {
	endDate.hours = rtc_get_hours(game_rtc);
	endDate.minutes = rtc_get_minutes(game_rtc);
	endDate.seconds = rtc_get_seconds(game_rtc);
	endDate.totalSeconds = endDate.hours*3600 + endDate.minutes*60 + endDate.seconds;
}

//********************************************* RANDOMIZES ***************************
int rand_(int min, int max) {
	return (rand() % max + min);
}

//********************************************* HANDLERS *****************************
int mouseHandle() {
	int x = mouse.x, y = mouse.y;
	int deltax, deltay;

	if(XSIGN(mouse.packet[0])) {
		deltax = mouse.packet[1] ^ 0xFFFFFF00;
	}
	else {
		deltax = mouse.packet[1];
	}

	if(YSIGN(mouse.packet[0])) {
		deltay = mouse.packet[2] ^ 0xFFFFFF00;
	}
	else {
		deltay = mouse.packet[2];
	}

	if(!(XOV(mouse.packet[0]) || YOV(mouse.packet[0]))) {
		x += deltax;
		y -= deltay;
	}

	if (x >= (signed int)(Xlimit)) {
		x = Xlimit - 1;
	}
	else if (x <= 0) {
		x = 0;
	}
	if (y >= (signed int)(Ylimit)) {
		y = Ylimit - 1;
	}
	else if (y <= 0) {
		y = 0;
	}
	mouse.x = x;
	mouse.y = y;

	if(LB(mouse.packet[0])) {
		mouse.buttonPressed = LBT;
	}
	else if(mouse.buttonPressed == LBT){
		mouse.buttonPressed = ZERO;
	}

	if(debug) {
		printf("x: %d; y: %d; button: %d\n", mouse.x, mouse.y, mouse.buttonPressed);
	}
	return 0;
}

int keyboardHandle() {

	if(debug) {
		printf("keyboard code: %x\n", keyboard.code);
	}

	if(keyboard.code != TWO_BYTE_CODE) {

		if(keyboard.code == MAKECODE_ESC) {
			return 1;
		}

		if(option == GAME) {
			if((keyboard.code == BREAKCODE_W) && (player.direction == UP)) {
				player.direction = STALL;
			}
			else if((keyboard.code == BREAKCODE_S) && (player.direction == DOWN)) {
				player.direction = STALL;
			}
			else if((keyboard.code == BREAKCODE_D) && (player.direction == RIGHT)) {
				player.direction = STALL;
			}
			else if((keyboard.code == BREAKCODE_A) && (player.direction == LEFT)) {
				player.direction = STALL;
			}
			else if(keyboard.code == MAKECODE_W) {
				player.direction = UP;
			}
			else if(keyboard.code == MAKECODE_S) {
				player.direction = DOWN;
			}
			else if(keyboard.code == MAKECODE_D) {
				player.direction = RIGHT;
			}
			else if(keyboard.code == MAKECODE_A) {
				player.direction = LEFT;
			}
		}
	}

	return 0;
}

int timerHandle() {
	timer.counter++;
	return 0;
}

int validPackets() {
	return (BIT3(mouse.packet[0]));
}

//********************************************* LEVEL ********************************
int increase_lvl() {
	lvl++;
	if(black_holes < 6) { // max of 6 black holes
		black_holes++;
	}
	if(safe_holes > 2) { // min 2 safe holes
		safe_holes--;
	}
	return 0;
}

int increase_lvl_difficulty() {
	difficulty++;
	red_square_speed++;
	black_holes = 0;
	safe_holes = 6;
	return 0;
}

//********************************************* PLAYER *******************************
int checkPlayerStatus() {
	if(player.power <= 0) {
		player.lives -= 1;
		player.power = 100;
	}
	if(player.lives == 0) {
		return -1;
	}
	return 0;
}

int checkPlayerCollision() {
	short int xCollision = 0, yCollision = 0;
	if(player.x < SCENARIO_X) {
		player.x = SCENARIO_X + 2;
		xCollision = 1;
	}
	else if ((player.x + player.size) > (SCENARIO_X + SCENARIO_SIZE)) {
		player.x = SCENARIO_X + SCENARIO_SIZE - player.size;
		xCollision = 1;
	}
	if(player.y < SCENARIO_Y) {
		player.y = SCENARIO_Y + 2;
		yCollision = 1;
	}
	else if ((player.y + player.size) > (SCENARIO_Y + SCENARIO_SIZE)) {
		player.y = SCENARIO_Y + SCENARIO_SIZE - player.size;
		yCollision = 1;
	}
	if(xCollision || yCollision) {
		player.direction = STALL;
	}
	return 0;
}

int returnPlayerCurrentDivision() {
	int realX = (player.xf + player.x) / 2;
	int realY = (player.yf + player.y) / 2;

	if((realX > SCENARIO_X_HOLE) && (realX < (SCENARIO_X_HOLE + SCENARIO_HOLE_SIZE)) &&
			(realY > SCENARIO_Y_HOLE) && (realY < (SCENARIO_Y_HOLE + SCENARIO_HOLE_SIZE))) {
		return -1;
	}

	/**
	 * |X| |
	 * |X| |
	 */
	if(realX < SCENARIO_X_MIDDLE) { // first half
		/**
		 * |X| |
		 * | | |
		 */
		if(realY < SCENARIO_Y_MIDDLE) { // first quadrant
			if(realX > realY) { // first octant
				return 0;
			}
			else {	// eighth octant
				return 7;
			}
		}
		/**
		 * | | |
		 * |X| |
		 */
		else {

			if(realX > (2*SCENARIO_Y_MIDDLE - realY)) {
				return 5;
			}
			else {
				return 6;
			}
		}
	}
	/**
	 * | |X|
	 * | |X|
	 */
	else { // second half
		/**
		 * | |X|
		 * | | |
		 */
		if(realY < SCENARIO_Y_MIDDLE) {
			if((2*SCENARIO_X_MIDDLE - realX) < realY) {
				return 2;
			}
			else {
				return 1;
			}
		}
		/**
		 * | | |
		 * | |X|
		 */
		else {
			if(realX > realY) {
				return 3;
			}
			else {
				return 4;
			}
		}
	}
	return -2;
}

int updatePlayerPosition() {

	if(player.direction == UP) {
		player.y -= player.ySpeed;
	}
	else if(player.direction == DOWN) {
		player.y += player.ySpeed;
	}
	else if(player.direction == RIGHT) {
		player.x += player.xSpeed;
	}
	else if(player.direction == LEFT){
		player.x -= player.xSpeed;
	}

	player.xf = player.x + player.size;
	player.yf = player.y + player.size;

	return 0;
}

int updatePlayerStatus() {
	int currentDisivion;
	int index;
	unsigned int currentColor;

	currentDisivion = returnPlayerCurrentDivision();

	if(currentDisivion >= 0) {
		currentColor = hole_color[currentDisivion];
		//debugTestWrite(currentColor);
		if(currentColor == colors_faded[WHITE]) {
			player.lives -= 1;
			player.score -= colors_takeaway[WHITE];
		}
		else if(currentColor != safe_color) {
			for(index = 1; index < colors_size; index++) {
				if(currentColor == colors[index]) {
					player.power -= colors_takeaway[index];
					player.score -= colors_takeaway[index];
					index = colors_size;
				}
			}
		}
	}
	else {
		if(currentDisivion == -1) {
			player.lives -= 1;
			player.score -= colors_takeaway[WHITE];
		}
		else {
			return -1;
		}
	}

	return 0;
}

int updatePlayerScore() {
	player.score += BASE_SCORE;
	if(player.score >= next_level_score) {
		increase_lvl();
		if(lvl == (difficulty*BASE_DIFF_MULT)) {
			increase_lvl_difficulty();
		}
		next_level_score = (lvl * NEXT_LEVEL_SCORE);
	}
	return 0;
}

int drawPlayer() {
	int i, j, indexLine = 0, indexCol = 0;
	unsigned int color;

	for(i = 0; i < player.height; i += 1) {
		for(j = 0; j < player.width; j += 1) {
			if(player.sprite[i][j] != ' ') {
				color = player_colors[(unsigned int)(player.sprite[i][j]) - 48];
				if(vg_set_pixel(player.x + indexCol, player.y + indexLine, color) == -1) {
					return -1;
				}
				if(vg_set_pixel(player.x + indexCol + 1, player.y + indexLine, color) == -1) {
					return -1;
				}
				if(vg_set_pixel(player.x + indexCol + 1, player.y + indexLine + 1, color) == -1) {
					return -1;
				}
				if(vg_set_pixel(player.x + indexCol, player.y + indexLine + 1, color) == -1) {
					return -1;
				}
			}
			indexCol += PLAYER_PIX_SIZE;
		}
		indexCol = 0;
		indexLine += PLAYER_PIX_SIZE;
	}
	return 0;
}

//********************************************* SCENARIO *****************************
int updateRedSquare() {
	red_square_index += red_square_speed;
	if(red_square_index >= (SCENARIOS_NUM - 1)) {
		red_square_index = 1;
	}
	return 0;
}

int updateSideMenu() {
	bars[LIVES].hSize = (player.lives/3.0)*(13*CHAR_SIZE + 12*CHAR_SPACE);
	switch(player.lives) {
	case 1:
		bars[LIVES].color = colors[RED];
		break;
	case 2:
		bars[LIVES].color = colors[YELLOW];
		break;
	case 3:
		bars[LIVES].color = colors[GREEN];
		break;
	default:
		break;
	}
	bars[ENERGY].hSize = (player.power/100.0)*(13*CHAR_SIZE + 12*CHAR_SPACE);
	if(player.power > 90) {
		bars[ENERGY].color = colors[BLUE];
	}
	else if(player.power > 70) {
		bars[ENERGY].color = colors[GREEN];
	}
	else if(player.power > 50) {
		bars[ENERGY].color = colors[YELLOW];
	}
	else if(player.power > 30) {
		bars[ENERGY].color = colors[ORANGE];
	}
	else if(player.power > 10) {
		bars[ENERGY].color = colors_faded[ORANGE];
	}
	else {
		bars[ENERGY].color = colors_faded[RED];
	}
	return 0;
}

int updateScenario() {
	int iter = 0;
	int randomNumer = 0;
	int randomColor = 0;

	int inner_iter = 0;

	safe_color = colors[rand_(1,(colors_size - 1))];
	bars[SAFE_COLOR].color = safe_color;

	clearHoleArray();

	iter = 0;
	/**
	 * Distribute safe holes.
	 */
	while(iter < safe_holes) {
		randomNumer = rand_(0,divisions);
		if(hole_color[randomNumer] != safe_color) {
			iter++;
			hole_color[randomNumer] = safe_color;
		}
		inner_iter++;
		if(inner_iter > reasonable_iterations) {;
			break;
		}
	}
	iter = 0;
	inner_iter = 0;
	/**
	 * Distribute black holes.
	 */
	while(iter < black_holes) {
		randomNumer = rand_(0,divisions);
		if((hole_color[randomNumer] != safe_color) && (hole_color[randomNumer] != colors_faded[WHITE])) {
			iter++;
			hole_color[randomNumer] = colors_faded[WHITE];
		}
		inner_iter++;
		if(inner_iter > reasonable_iterations) {
			break;
		}
	}
	iter = 0;
	/**
	 * Distribute remaining holes.
	 */
	for(;iter < divisions; iter++) {
		if((hole_color[iter] != safe_color) && (hole_color[iter] != colors_faded[WHITE])) {
			do{
				randomColor = colors[rand_(1,(colors_size - 1))];
			}while(randomColor == safe_color);
			hole_color[iter] = randomColor;
		}
	}

	return 0;
}

int drawScenario() {
	unsigned int index;
	unsigned int inner_index;

	if(vg_draw_square(scenario[SCENARIOS_NUM - 1].x,scenario[SCENARIOS_NUM - 1].y,scenario[SCENARIOS_NUM - 1].size,red_square_color) == -1) {
		drawError("error in first drawsquare.");
		sleep(2);
		return -1;
	}

	if(!debug) {
		for(index = 1; index <= red_square_index; index++) {
			if(index < red_square_index) {
				if(vg_draw_death_square(scenario[index].x,scenario[index].y,scenario[index].size,hole_color) == -1) {
					drawError("error in drawdeathsquare.");
					sleep(2);
					return -1;
				}
			}
			else if(index == red_square_index) {
				for(inner_index = 0 ; (inner_index < red_square_size) && ((index + inner_index) < SCENARIOS_NUM); inner_index++ ) {
					if(vg_draw_square(scenario[index + inner_index].x,
							scenario[index + inner_index].y,
							scenario[index + inner_index].size,
							red_square_color) == -1) {
						drawError("error in drawredsquare.");
						sleep(2);
						return -1;
					}
				}
//				index += red_square_size;
			}
		}
	}
	return 0;
}

int drawSideMenu() {
	unsigned int index = 0;
	if(!debug) {
		for(index = LIVES; index <= SAFE_COLOR; index++) {
			if(drawString(bars[index].x, bars[index].y, bars[index].title,0,0) == -1) {
				return -1;
			}
			if(vg_fill_section(bars[index].x, bars[index].y + BAR_HSPACE,
					bars[index].x + bars[index].hSize, bars[index].y + BAR_HEIGHT + bars[index].vSize,
					bars[index].color) == -1) {
				return -1;
			}
			if(vg_draw_rectangle(bars[index].x-1, bars[index].y-1 + BAR_HSPACE,
					bars[index].x + BAR_LENGTH, bars[index].y + BAR_HEIGHT + bars[index].vSize,
					colors[WHITE]) == -1) {
				return -1;
			}
		}
		if(drawString(bars[SCORE].x, bars[SCORE].y, bars[SCORE].title,0,0) == -1) {
			return -1;
		}
		if(drawNumber(bars[SCORE].x + bars[SCORE].hSize, bars[SCORE].y + CHAR_SPACE, player.score, (player.score > 0) ? colors[GREEN] : colors[RED],0) == -1) {
			return -1;
		}
		if(drawString(bars[LEVEL].x, bars[LEVEL].y, bars[LEVEL].title,0,0) == -1) {
			return -1;
		}
		if(drawNumber(bars[LEVEL].x + bars[LEVEL].hSize, bars[LEVEL].y + CHAR_SPACE, lvl, bars[LEVEL].color,0) == -1) {
			return -1;
		}
		if(drawString(bars[NEXT_LVL].x, bars[NEXT_LVL].y, bars[NEXT_LVL].title,0,0) == -1) {
			return -1;
		}
		if(drawNumber(bars[NEXT_LVL].x + bars[NEXT_LVL].hSize, bars[NEXT_LVL].y + CHAR_SPACE, next_level_score, bars[NEXT_LVL].color,0) == -1) {
			return -1;
		}
	}
	return 0;
}

int drawBufferToScreen() {
	vg_draw_buffer_to_mem();
	return 0;
}

int clearBuffer(unsigned int color) {
	vg_fill(color);
	return 0;
}

int clearHoleArray() {
	int iter = 0;
	for(; iter < divisions; iter++) {
		hole_color[iter] = -1;
	}
	return 0;
}

//********************************************* MOUSE/CURSOR *************************
int isMouseInsideTitle() {
	unsigned int index = 0;
	for(index = 1; index <= QUIT; index++) {
		if(titles[index].visible) {
			if( (mouse.x >= titles[index].x) &&
					(mouse.x <= (titles[index].x + titles[index].hSize)) &&
					(mouse.y >= titles[index].y) &&
					(mouse.y <= (titles[index].y + titles[index].vSize))) {
				return index;
			}
		}
	}
	return 0;
}

int drawCursor() {
	int i, j, indexLine = 0, indexCol = 0;
	int cursMult = CURSOR_MULT/2;
	int index1, index2;

	for(i = 0; i < cursor.width; i++) {
		for(j = 0; j < cursor.width; j++) {
			if(cursor.sprite[i][j] != ' ') {
				for(index1 = 0; index1 < cursMult; index1++) {
					for(index2 = 0; index2 < cursMult; index2++) {
						if(vg_set_pixel(mouse.x + indexCol + index2, mouse.y + indexLine + index1, (cursor.sprite[i][j])) == -1) {
							return -1;
						}
					}
				}
			}
			indexCol += CURSOR_PIX_SIZE;
		}
		indexCol = 0;
		indexLine += CURSOR_PIX_SIZE;
	}
	return 0;
}

//********************************************* MENU *********************************
int drawFrame() {
	int index;
	for(index = 1; index <= 4; index += 1) {
		if(vg_draw_rectangle(index, index, (HMAX - index), (VMAX - index), CHAR_COLOR) == -1) {
			return -1;
		}
	}
	return 0;
}

int drawMenu() {
	unsigned int index = 0;
	int x,y;

	if(drawMenuDate() == -1) {
		return -1;
	}

	for(index = 0; index <= QUIT; index++) {
		if(titles[index].visible) {
			x = titles[index].x;
			y = titles[index].y;
			if(drawString(x,y,titles[index].title,0,0) == -1) {
				return -1;
			}
		}
	}

	return 0;
}

int drawMenuDate() {
	int x = (0.70 * HMAX);
	int y = (0.10 * VMAX);

	int returnValue;

	int month_day = rtc_get_month_day(game_rtc);
	int hours = rtc_get_hours(game_rtc);
	int min = rtc_get_minutes(game_rtc);
	int sec = rtc_get_seconds(game_rtc);

	if (month_day < 10) {
		returnValue = drawNumber(x,y,0,7,10);
		if(returnValue == -1) {
			return -1;
		}
		else {
			x += returnValue;
		}
		x += 10;
	}
	returnValue = drawNumber(x,y,month_day,7,10);
	if(returnValue == -1) {
		return -1;
	}
	else {
		x += returnValue;
	}
	x += 20;

	returnValue = drawString(x,y,rtc_get_month(game_rtc),6,7);
	if(returnValue == -1) {
		return -1;
	}
	else {
		x += returnValue;
	}

	if (hours < 10) {
		returnValue = drawNumber(x,y,0,7,10);
		if(returnValue == -1) {
			return -1;
		}
		else {
			x += returnValue;
		}
	}
	returnValue = drawNumber(x,y,hours,7,10);
	if(returnValue == -1) {
		return -1;
	}
	else {
		x += returnValue;
	}
	x += 14;

	if (min < 10) {
		returnValue = drawNumber(x,y,0,7,10);
		if(returnValue == -1) {
			return -1;
		}
		else {
			x += returnValue;
		}
		x += 10;
	}
	returnValue = drawNumber(x,y,min,7,10);
	if(returnValue == -1) {
		return -1;
	}
	else {
		x += returnValue;
	}
	x += 14;

	if (sec < 10) {
		returnValue = drawNumber(x,y,0,7,10);
		if(returnValue == -1) {
			return -1;
		}
		else {
			x += returnValue;
		}
		x += 10;
	}
	returnValue = drawNumber(x,y,sec,7,10);
	if(returnValue == -1) {
		return -1;
	}
	else {
		x += returnValue;
	}

	return 0;
}

int drawMenuHelp() {
	return drawString(HELP_X,HELP_Y, "the game consists in escaping the bad colors::"
							 	 "each round a safe color is chosen::"
							 	 "if the player's figure is not on that color:"
							 	 "upon the round's ending the player loses:"
							 	 "a power or power, depending on the color of:"
							 	 "the tile the player is in::"
							 	 "(a round is the time that the red square:"
							 	 "takes to fill the game board)::use 'a' 'w' 's' 'd' to control the figure::"
							 	 "exit   (esc).",0,0);
}

int drawMenuOptions() {
	return drawString(OPTIONS_X,OPTIONS_Y," there are no options actually.",0,0);
}

int drawMenuCredits() {
	return drawString(CREDITS_X,CREDITS_Y,
								   " created by:::"
								   "  kevin amorim::"
								   "  luis magalhaes::"
								   "  marcio fontes:::::"
								   "  if you are enlightened press esc:  to return to menu.",0,0);
}

int drawLoserScreen() {
	int x = LOSER_X;
	int y = LOSER_Y;
	clearBuffer(back_color);
	if(drawString(x,y,"you kinda lost.",0,0) == -1) {
		return -1;
	}
	y += 8*CHAR_SIZE;
	x += drawString(x,y,"score.",0,0);
	x += 2*CHAR_SPACE;
	y += 11;
	if(drawNumber(x,y,player.score,colors[GREEN],0) == -1) {
		return -1;
	}
	x = LOSER_X;
	y += 8*CHAR_SIZE;
	x += drawString(x,y,"time played (sec).",0,0);
	x += 2*CHAR_SPACE;
	y += 11;
	if(drawNumber(x,y,getPlayTime(),colors[GREEN],0) == -1) {
		return -1;
	}
	drawBufferToScreen();
	return 0;
}

int drawQuitScreen() {
	int x = LOSER_X;
	int y = LOSER_Y;
	clearBuffer(back_color);
	if(drawString(x,y,"you quitter.",0,0) == -1) {
		return -1;
	}
	y += 8*CHAR_SIZE;
	x += drawString(x,y,"score.",0,0);
	x += 2*CHAR_SPACE;
	y += 11;
	if(drawNumber(x,y,player.score,colors[GREEN],0) == -1) {
		return -1;
	}
	x = LOSER_X;
	y += 8*CHAR_SIZE;
	x += drawString(x,y,"time played (sec).",0,0);
	x += 2*CHAR_SPACE;
	y += 11;
	if(drawNumber(x,y,getPlayTime(),colors[GREEN],0) == -1) {
		return -1;
	}
	drawBufferToScreen();
	return 0;
}

long int getPlayTime() {
	return (endDate.totalSeconds - startDate.totalSeconds);
}

//********************************************* INTERRUPTS ***************************
int handleInterrupts(unsigned int * events) {

	if((*events) & TIMER_IRQ_SET) {

		if(timer.counter % 60) {
			game_rtc = rtc_get_date();
		}

		switch(option) {
		case GAME:
			clearBuffer(0);
			if(drawFrame() == -1) {
				return -3;
			}
			updateRedSquare();
			if(drawString((0.20*HMAX),(0.92*VMAX),"keep in the safe color.",6,0) == -1) {
				drawError("error in drawstring 'keep in the safe color'.");
				return -3;
			}

			if(red_square_index == 1) {
				if(first_round) {
					first_round = 0;
				}
				else {
					if(updatePlayerStatus() == -1) {
						drawError("error in updateplayerstatus.");
						return -3;
					}
					if(checkPlayerStatus() == -1) { /* player has lost all of its lives */
						keyboard_set_leds(player.lives);
						return -1;
					}
				}
				updatePlayerScore();
				updateScenario();
				updateSideMenu();
			}

			updatePlayerPosition();
			checkPlayerCollision();

			keyboard_set_leds(player.lives);

			if(drawSideMenu() == -1) {
				drawError("error in drawsidemenu.");
				return -3;
			}
			if(drawScenario() == -1) {
				drawError("error in drawscenario.");
				return -3;
			}
			if(drawPlayer() == -1) {
				drawError("error in drawplayer.");
				return -3;
			}

			break;
		case MENU:
			clearBuffer(back_color);
			if(drawFrame() == -1) {
				return -3;
			}
			if(drawMenu() == -1) {
				drawError("error in drawsidemenu.");
				return -4;
			}
			if(drawCursor() == -1) {
				drawError("error in drawsidemenu.");
				return -4;
			}
			break;
		case MENU_HELP:
			clearBuffer(back_color);
			if(drawFrame() == -1) {
				return -3;
			}
			if(drawMenuHelp() == -1) {
				drawError("error in drawsidemenu.");
				return -4;
			}
			if(drawCursor() == -1) {
				drawError("error in drawsidemenu.");
				return -4;
			}
			break;
		case MENU_OPTIONS:
			clearBuffer(back_color);
			if(drawFrame() == -1) {
				return -3;
			}
			if(drawMenuOptions() == -1) {
				drawError("error in drawsidemenu.");
				return -4;
			}
			if(drawCursor() == -1) {
				drawError("error in drawsidemenu.");
				return -4;
			}
			break;
		case MENU_CREDITS:
			clearBuffer(back_color);
			if(drawFrame() == -1) {
				return -3;
			}
			if(drawMenuCredits() == -1) {
				drawError("error in drawsidemenu.");
				return -4;
			}
			if(drawCursor() == -1) {
				drawError("error in drawsidemenu.");
				return -4;
			}
			break;
		default:
			break;
		}
		drawBufferToScreen();
	}
	if((*events) & KEYBOARD_IRQ_SET) {
		if(keyboardHandle() == 1) {
			switch(option) {
			case MENU:
				return 1;
				break;
			case MENU_HELP:
				option = MENU;
				break;
			case MENU_OPTIONS:
				option = MENU;
				break;
			case MENU_CREDITS:
				option = MENU;
				break;
			case GAME:
				return -2;
				break;
			default:
				option = MENU;
				break;
			}
		}
	}
	if((*events) & MOUSE_IRQ_SET) {
		if(mouse.packetCounter == 3) {
			mouseHandle();
			mouse.packetCounter = 0;
		}
		if(option != GAME) {
			if(option == MENU) {
				if(mouse.buttonPressed == LBT) {
					switch(isMouseInsideTitle()) {
					case PLAY:
						getStartTime();
						option = GAME;
						break;
					case HELP:
						option = MENU_HELP;
						break;
					case OPTIONS:
						option = MENU_OPTIONS;
						break;
					case CREDITS:
						option = MENU_CREDITS;
						break;
					case QUIT:
						return 1;
					default:
						break;
					}
				}
			}
		}
	}

	return 0;
}

//********************************************* VARIABLE INITIALIZATION **************
int initVars() {

	/* misc */
	option = MENU;

	/* timer */
	timer.counter = 0;

	// scenario
	int size = 0, index = SCENARIOS_NUM - 1, size_index = 0;
	for(size = SCENARIO_SIZE; index > 0; (size = size - 2*SCENARIO_WIDTH), size_index++, index--) {
		scenario[index].x = SCENARIO_X + (size_index*SCENARIO_WIDTH);
		scenario[index].y = SCENARIO_Y + (size_index*SCENARIO_WIDTH);
		scenario[index].size = size;
	}

	clearHoleArray();

	// level
	lvl = 1;
	difficulty = 1;
	next_level_score = (lvl + difficulty - 1) * NEXT_LEVEL_SCORE;

	// player
	player.power = 100;
	player.lives = 3;
	player.score = 0;
	//
	player.width = 21;
	player.height = 21;
	//
	player.xSpeed = 10;
	player.ySpeed = 10;
	player.direction = STALL;
	player.size = PLAYER_PIX_SIZE*player.width;
	player.x = SCENARIO_X_MIDDLE;
	player.y = SCENARIO_Y_MIDDLE;
	player.xf = player.x + player.size;
	player.yf = player.y + player.size;

	// mouse
	mouse.x = 1;
	mouse.y = 1;

	// cursor
	cursor.width = 5;
	cursor.size = cursor.width*CURSOR_PIX_SIZE;

	// titles
	char *str0 = malloc(10);
	char *str1 = malloc(10);
	char *str2 = malloc(10);
	char *str3 = malloc(10);
	char *str4 = malloc(10);
	char *str5 = malloc(10);
	char *str6 = malloc(10);
	char *str7 = malloc(10);
	char *str8 = malloc(10);
	char *str9 = malloc(10);
	char *str10 = malloc(10);
	char *str11 = malloc(10);

	strcpy(str0, "black division.");
	titles[TITLE].x = MENU_X;
	titles[TITLE].y = MENU_Y * (TITLE + 1);
	titles[TITLE].title = str0;
	titles[TITLE].hSize = BAR_LENGTH;
	titles[TITLE].vSize = BAR_HEIGHT;
	titles[TITLE].visible = 1;

	strcpy(str1, "play.");
	titles[PLAY].x = MENU_X;
	titles[PLAY].y = MENU_Y * (PLAY + 1);
	titles[PLAY].title = str1;
	titles[PLAY].hSize = BAR_LENGTH;
	titles[PLAY].vSize = BAR_HEIGHT;
	titles[PLAY].visible = 1;

	strcpy(str11, "options.");
	titles[OPTIONS].x = MENU_X;
	titles[OPTIONS].y = MENU_Y * (OPTIONS + 1);
	titles[OPTIONS].title = str11;
	titles[OPTIONS].hSize = BAR_LENGTH;
	titles[OPTIONS].vSize = BAR_HEIGHT;
	titles[OPTIONS].visible = 1;

	strcpy(str2, "help.");
	titles[HELP].x = MENU_X;
	titles[HELP].y = MENU_Y * (HELP + 1);
	titles[HELP].title = str2;
	titles[HELP].hSize = BAR_LENGTH;
	titles[HELP].vSize = BAR_HEIGHT;
	titles[HELP].visible = 1;

	strcpy(str3, "credits.");
	titles[CREDITS].x = MENU_X;
	titles[CREDITS].y = MENU_Y * (CREDITS + 1);
	titles[CREDITS].title = str3;
	titles[CREDITS].hSize = BAR_LENGTH;
	titles[CREDITS].vSize = BAR_HEIGHT;
	titles[CREDITS].visible = 1;

	strcpy(str4, "quit.");
	titles[QUIT].x = MENU_X;
	titles[QUIT].y = MENU_Y * (QUIT + 1);
	titles[QUIT].title = str4;
	titles[QUIT].hSize = BAR_LENGTH;
	titles[QUIT].vSize = BAR_HEIGHT;
	titles[QUIT].visible = 1;

	strcpy(str5, "lives.");
	bars[LIVES].x = GAME_X;
	bars[LIVES].y = GAME_Y * (LIVES + 1);
	bars[LIVES].title = str5;
	bars[LIVES].hSize = BAR_LENGTH;
	bars[LIVES].vSize = BAR_HEIGHT;
	bars[LIVES].color = colors[GREEN];
	bars[LIVES].visible = 1;

	strcpy(str6, "power.");
	bars[ENERGY].x = GAME_X;
	bars[ENERGY].y = GAME_Y * (ENERGY + 1);
	bars[ENERGY].title = str6;
	bars[ENERGY].hSize = BAR_LENGTH;
	bars[ENERGY].vSize = BAR_HEIGHT;
	bars[ENERGY].color = colors[BLUE];
	bars[ENERGY].visible = 1;

	strcpy(str7, "safe color.");
	bars[SAFE_COLOR].x = GAME_X;
	bars[SAFE_COLOR].y = GAME_Y * (SAFE_COLOR + 1);
	bars[SAFE_COLOR].title = str7;
	bars[SAFE_COLOR].hSize = BAR_LENGTH;
	bars[SAFE_COLOR].vSize = 8*BAR_HEIGHT;
	bars[SAFE_COLOR].color = colors_faded[WHITE];
	bars[SAFE_COLOR].visible = 1;

	strcpy(str8, "score.");
	bars[SCORE].x = GAME_X;
	bars[SCORE].y = GAME_Y * (SCORE + 1) + bars[SAFE_COLOR].vSize;
	bars[SCORE].title = str8;
	bars[SCORE].hSize = (BAR_LENGTH/2);
	bars[SCORE].vSize = BAR_HEIGHT;
	bars[SCORE].color = colors[WHITE];
	bars[SCORE].visible = 1;

	strcpy(str9, "level.");
	bars[LEVEL].x = GAME_X;
	bars[LEVEL].y = GAME_Y * (LEVEL + 1) + bars[SAFE_COLOR].vSize;
	bars[LEVEL].title = str9;
	bars[LEVEL].hSize = (BAR_LENGTH/2);
	bars[LEVEL].vSize = BAR_HEIGHT;
	bars[LEVEL].color = colors[WHITE];
	bars[LEVEL].visible = 1;

	strcpy(str10, "next level.");
	bars[NEXT_LVL].x = GAME_X;
	bars[NEXT_LVL].y = GAME_Y * (NEXT_LVL + 1) + bars[SAFE_COLOR].vSize;
	bars[NEXT_LVL].title = str10;
	bars[NEXT_LVL].hSize = (3*BAR_LENGTH/4);
	bars[NEXT_LVL].vSize = BAR_HEIGHT;
	bars[NEXT_LVL].color = colors[WHITE];
	bars[NEXT_LVL].visible = 1;

	keyboard_set_leds(player.lives);

	return 0;
}

int resetGameVars() {

	clearHoleArray();
	black_holes = 0;
	safe_holes = 6;

	// level
	lvl = 1;
	difficulty = 1;
	next_level_score = (lvl + difficulty - 1) * NEXT_LEVEL_SCORE;

	//red square
	red_square_index = 2;
	red_square_color = 4;
	red_square_speed = 2;

	//
	titles[SAFE_COLOR].color = colors_faded[WHITE];

	titles[LIVES].hSize = BAR_LENGTH;
	titles[LIVES].color = colors[GREEN];

	titles[ENERGY].hSize = BAR_LENGTH;
	titles[ENERGY].color = colors[BLUE];

	//player
	player.power = 100;
	player.lives = 3;
	player.score = 0;

	return 0;
}

//********************************************* MAIN LOOPS ***************************
int gameLoop(const short int debugmode) {
	unsigned int events = 0;
	int toBreak = 0;
	int ipc_status;
	message msg;

	debug = debugmode;

	initVars();

	while(!toBreak) {

		if ( driver_receive(ANY, &msg, &ipc_status) == 0 ) {

			if (is_ipc_notify(ipc_status)) {

				switch (_ENDPOINT_P(msg.m_source)) {

				case HARDWARE:
					if (msg.NOTIFY_ARG & TIMER_IRQ_SET) { // TIMER interrupt
						timer.counter++;
						events = events | TIMER_IRQ_SET;
					}
					if (msg.NOTIFY_ARG & KEYBOARD_IRQ_SET) { // KEYBOARD interrupt
						keyboard.code = keyboard_scan();
						events = events | KEYBOARD_IRQ_SET;
					}
					if (msg.NOTIFY_ARG & MOUSE_IRQ_SET) { // MOUSE interrupt
						mouse.packet[mouse.packetCounter] = mouse_receive_packet();
						if(validPackets()) {
							mouse.packetCounter++;
						}
						else {
							mouse.packetCounter = 0;
						}
						events = events | MOUSE_IRQ_SET;
					}
					break;
				default:
					break;
				}
			}
			toBreak = handleInterrupts(&events);
			events = 0;
		}

		if(toBreak < 0) {
			if(toBreak > -3) { // player has lost/quit
				getEndTime();
				(toBreak == -1) ? drawLoserScreen() : drawQuitScreen();
				sleep(2);
				resetGameVars();
				toBreak = 0;
				option = MENU;
			}
			else if(toBreak == -3) {
				sleep(2);
				resetGameVars();
				toBreak = 0;
				option = MENU;
			}
			else if(toBreak == -4) {
				sleep(2);
				toBreak = 1;
			}
		}
	}

	keyboard_set_leds(ZERO);
	return 0;
}




