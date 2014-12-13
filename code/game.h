#ifndef GAME_H_
#define GAME_H_

#include "libraries.h"

/** @defgroup game game
 * @{
 *
 * Functions related to the game logic and graphics.
 */

/* MISC */
#define	TIMER_ID	0	/**< @brief timer0's ID */
#define KEYBOARD_ID	1	/**< @brief keyboard's ID */
#define	MOUSE_ID	2	/**< @brief mouse's ID */
#define	SPEAKER_ID	3	/**< @brief speaker's ID */
#define	RTC_ID		4	/**< @brief rtc's ID */

#define	BASE_SCORE			100		/**< @brief base score won by round */
#define	NEXT_LEVEL_SCORE	200		/**< @brief base score to multiply by level and difficulty to define the next level */
#define BASE_DIFF_MULT		8		/**< @brief multiplier associated with the difficulty's increase ratio */

/* MENU CONTENT */
#define	MENU_X		(0.10 * HMAX)	/**< @brief Main Menu initial x border */
#define MENU_Y		(0.10 * VMAX)	/**< @brief Main Menu initial y border */

#define	HELP_X		(0.05 * HMAX)	/**< @brief Help Menu initial x border */
#define HELP_Y		(0.05 * VMAX)	/**< @brief Help Menu initial y border */

#define	OPTIONS_X	(0.05 * HMAX)	/**< @brief Options Menu initial x border */
#define OPTIONS_Y	(0.05 * VMAX)	/**< @brief Options Menu initial y border */

#define	CREDITS_X	(0.05 * HMAX)	/**< @brief Credits Menu initial x border */
#define CREDITS_Y	(0.05 * VMAX)	/**< @brief Credits Menu initial y border */

#define	GAME_X		(0.70 * HMAX)	/**< @brief Game initial x border */
#define GAME_Y		(0.10 * VMAX)	/**< @brief Game initial y border */

#define	LOSER_X		(0.05 * HMAX)	/**< @brief Loser/Quitter Menu initial x border */
#define LOSER_Y		(0.02 * VMAX)	/**< @brief Loser/Quitter Menu initial y border */

#define CHAR_COLOR		55	/**< @brief Default color for characters */
#define CHAR_SIZE		8	/**< @brief Default size for characters */
#define CHAR_SPACE		12	/**< @brief Default space between non-numerical characters */
#define NUM_SPACE		14	/**< @brief Default space between numerical characters */

#define	BAR_SPACE	(0.05 * VMAX)	/**< @brief Default space between bars */
#define	BAR_LENGTH	(13*CHAR_SIZE + 12*CHAR_SPACE) /**< @brief Default bar length */
#define	BAR_HEIGHT	(4*CHAR_SIZE)	/**< @brief Default bar height */
#define	BAR_HSPACE	(6*CHAR_SIZE)	/**< @brief Default bar horizontal occupational space */

/* MOUSE CONTENT */
#define CURSOR_MULT			4	/**< @brief Multiplier of the cursor's size */
#define CURSOR_PIX_SIZE		2	/**< @brief Multiplier of the cursor's sprite size */
#define Xlimit		(HMAX - cursor.size)	/**< @brief Cursor's x border limit */
#define Ylimit		(VMAX - cursor.size)	/**< @brief Cursor's y border limit */

#define LBT		1	/**< @brief Left Mouse Button */
#define RBT		2	/**< @brief Right Mouse Button */
#define MBT		4	/**< @brief Middle Mouse Button */
#define	ZERO	0	/**< @brief No Mouse Button */

/* KEYBOARD CONTENT */
#define TWO_BYTE_CODE	0xE0	/**< @brief Keyboard's two byte makecode */

#define MAKECODE_W		0x11	/**< @brief Keyboard's W key makecode */
#define MAKECODE_A		0x1e	/**< @brief Keyboard's A key makecode */
#define MAKECODE_S		0x1f	/**< @brief Keyboard's S key makecode */
#define MAKECODE_D		0x20	/**< @brief Keyboard's D key makecode */
#define MAKECODE_ESC	0x81	/**< @brief Keyboard's ESC key makecode */

#define BREAKCODE_W		0x91	/**< @brief Keyboard's W key breakcode */
#define BREAKCODE_A		0x9e	/**< @brief Keyboard's A key breakcode */
#define BREAKCODE_S		0x9f	/**< @brief Keyboard's S key breakcode */
#define BREAKCODE_D		0xa0	/**< @brief Keyboard's D key breakcode */

/* SCENARIO CONTENT */
#define	SCENARIO_X			40
#define	SCENARIO_Y			40
#define	SCENARIO_X_END		680
#define	SCENARIO_Y_END		680
#define SCENARIO_X_MIDDLE	360
#define SCENARIO_Y_MIDDLE	360
#define SCENARIO_X_HOLE		((SCENARIO_X + SCENARIOS_NUM*SCENARIO_WIDTH) - 1)
#define SCENARIO_Y_HOLE		((SCENARIO_Y + SCENARIOS_NUM*SCENARIO_WIDTH) - 1)
#define SCENARIO_HOLE_SIZE	( ( (SCENARIOS_NUM_F - SCENARIOS_NUM) * SCENARIO_WIDTH) * 2 + 3)

#define	SCENARIO_SIZE	640
#define	SCENARIO_WIDTH	1
#define	SCENARIOS_NUM_F	(SCENARIO_SIZE / 2)
#define SCENARIOS_NUM	250

/* PLAYER CONTENT */
#define PLAYER_MULT			4
#define PLAYER_PIX_SIZE		2


/* FUNCTIONS */

/**
 * @brief Writes a number in the position (x,y). ONLY FOR DEBUG.
 *
 * @param x coordinate on the x axis
 * @param y coordinate on the y axis
 * @param number number to be written
 *
 * @return 0 if success, -1 otherwise
 */
int debugTestWrite(int x, int y, int number);
/**
 * @brief Writes a string in the position (x,y).
 *
 * @param x coordinate on the x axis
 * @param y coordinate on the y axis
 * @param str pointer to the string
 * @param size character size
 * @param color character color
 *
 * @return 0 if success, -1 otherwise
 */
int drawString(int x, int y, char * str, int size, unsigned int color);
/**
 * @brief Writes a number in the position (x,y).
 *
 * @param x coordinate on the x axis
 * @param y coordinate on the y axis
 * @param number number to be written
 * @param color character color
 * @param size character size
 *
 * @return 0 if success, -1 otherwise
 */
int drawNumber(int x, int y, long int number, unsigned int color, int size);
/**
 * @brief Fills a region of the buffer with the given color.
 *
 * @param x initial coordinate on the x axis
 * @param y initial coordinate on the y axis
 * @param xf final coordinate on the x axis
 * @param yf final coordinate on the y axis
 * @param color cor a preencher
 *
 * @return 0 if success
 */
int clearScreen(unsigned int x, unsigned int y, unsigned int xf, unsigned int yf, unsigned int color);
/**
 * @brief Draws an error on the buffer. ONLY FOR DEBUG.
 *
 * @param str pointer to the string to write as error
 *
 * @return 0 if success, -1 otherwise
 */
int drawError(char * str);
/**
 * @brief Updates struct startDate with the data from the Real Time Clock (rtc) in the beginning of the game.
 */
void getStartTime();
/**
 * @brief Updates struct endDate with the data from the Real Time Clock (rtc) in the ending of the game.
 */
void getEndTime();
/**
 * @brief Returns a random number in the interval [min;max].
 *
 * @param min interval's minimum
 * @param max interval's maximum
 *
 * @return random number in the interval
 */
int rand_(int min, int max);
/**
 * @brief Handles the data received from the mouse upon an interrupt.
 *
 * @return 0 if success
 */
int mouseHandle();
/**
 * @brief Handles the data received from the keyboard upon an interrupt.
 *
 * @return 1 if ESC is pressed, 0 otherwise
 */
int keyboardHandle();
/**
 * @brief Handles the data received from the timer upon an interrupt.
 *
 * @return 0 if success
 */
int timerHandle();
/**
 * @brief Checks whether the packets received from the mouse are valid or not.
 *
 * @return 1 if true, 0 if false
 */
int validPackets();
/**
 * @brief Increases the level of the game.
 *
 * @return 0 if success
 */
int increase_lvl();
/**
 * @brief Increases the difficulty of the game. (red square speed).
 *
 * @return 0 if success
 */
int increase_lvl_difficulty();
/**
 * @brief Checks whether the player as lost a life and if it has no remaining lives.
 *
 * @return 0 if success, 1 if the player has lost all of its lives.
 */
int checkPlayerStatus();
/**
 * @brief Checks whether the player is in the playable limits of the game board.
 *
 * @return 0 if success
 */
int checkPlayerCollision();
/**
 * @brief Returns the current division of the player's sprite. The game board is divided in 8 divisions/octants.
 *
 * @return player current division
 */
int returnPlayerCurrentDivision();
/**
 * @brief Updates the player's current position (x,y).
 *
 * @return 0 if success
 */
int updatePlayerPosition();
/**
 * @brief Checks whether the player is in a safe division. If not, decreases the player's power with the takeaway value associated with each color.
 *
 * @return 0 if success, -1 otherwise
 */
int updatePlayerStatus();
/**
 * @brief Updates the player's score upon each round's ending.
 *
 * @return 0 if success
 */
int updatePlayerScore();
/**
 * @brief Draws the player's sprite on the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawPlayer();
/**
 * @brief Updates the red square position.
 *
 * @return 0 if success
 */
int updateRedSquare();
/**
 * @brief Updates the game right side menu, which contains all the important game informations.
 *
 * @return 0 if success
 */
int updateSideMenu();
/**
 * @brief Updates the game board, calculating the safe color and the remaining divisions.
 *
 * @return 0 if success
 */
int updateScenario();
/**
 * @brief Draws the scenario to the buffer.

 * @return 0 if success, -1 otherwise
 */
int drawScenario();
/**
 * @brief Draws the game right side menu to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawSideMenu();
/**
 * @brief Abstraction. Calls vg_draw_buffer_to_mem().
 *
 * @return 0 if success
 */
int drawBufferToScreen();
/**
 * @brief Cleans the video buffer, calling the function vg_fill() with the parameter 0 (zero).
 *
 * @return 0 if success
 */
int clearBuffer(unsigned int color);
/**
 * @brief Clean the array containing the game divisions colors.
 *
 * @return 0 if success
 */
int clearHoleArray();
/**
 * @brief Checks whether the cursor is inside any of the menu titles.
 *
 * @return index of the title, 0 if not inside any
 */
int isMouseInsideTitle();
/**
 * @brief Draws the cursor to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawCursor();
/**
 * @brief Draws the menu frame to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawFrame();
/**
 * @brief Draws the main menu to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawMenu();
/**
 * @brief Draws the date and hour to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawMenuDate();
/**
 * @brief Draws the help menu to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawMenuHelp();
/**
 * @brief Draws the options menu to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawMenuOptions();
/**
 * @brief Draws the credits menu to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawMenuCredits();
/**
 * @brief Draws the loser screen to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawLoserScreen();
/**
 * @brief Draws the quitter screen to the buffer.
 *
 * @return 0 if success, -1 otherwise
 */
int drawQuitScreen();
/**
 * @brief Gets the play time, using the information in the structs startDate and endDate.
 *
 * @return play time
 */
long int getPlayTime();
/**
 * @brief Handles all interruptions and game order logic.
 *
 * @param events value with the interruptions (each 'bit' represents an interruption)
 *
 * @return 0 if success, -1 if player has lost, -2 if player as given up, errors otherwise
 */
int handleInterrupts(unsigned int * events);
/**
 * @brief Initializes the game variables.
 *
 * @return 0 if success
 */
int initVars();
/**
 * @brief Resets the game variables that were changed during gameplay.
 *
 * @return 0 if success
 */
int resetGameVars();
/**
 * @brief Main game loop. Receives the interrupts and calls handleInterrupts().
 *
 * @param debugmode variable which represents the value of debugmode (0 = 0FF, 1 = ON)
 *
 * @return 0 if success, -1 otherwise
 */
int gameLoop(const short int debugmode);

#endif	/* GAME_H_ */
