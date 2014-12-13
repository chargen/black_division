#ifndef I8042_H_
#define I8042_H_

#include "libraries.h"

/**
 * IRQ Lines.
 */
#define KBC_IRQ 		1								/**< Keyboard IRQ Line. */
#define MOUSE_IRQ 		12								/**< Mouse IRQ Line. */

/**
 * Errors Check.
 */
#define OBF 			BIT(0)							/**< Output Buffer Full. */
#define IBF				BIT(1)							/**< Input Buffer Full. */
#define TO_ERROR 		BIT(6)							/**< Timeout Error. */
#define PAR_ERROR 		BIT(7)							/**< Parity Error. */

/**
 * Registers.
 */
#define STAT_REG 		0x64							/**< Stat Register. */
#define KBC_CMD_REG 	0x64							/**< Keyboard Command Register. */
#define IN_BUF 			0x60							/**< Input Buffer Register. */
#define OUT_BUF 		0x60							/**< Output Buffer Register. */

/**
 * Mouse Commands.
 */
#define MOUSE_DISABLE_STREAM	0xF5					/**< Mouse disable stream command. */
#define MOUSE_RESET				0xFF					/**< Mouse reset command. */
#define MOUSE_ENABLE_STREAM		0xF4					/**< Mouse enable stream command. */
#define MOUSE_SET_STREAM_MODE	0xEA					/**< Mouse set stream mode command. */
#define MOUSE_STATUS_REQUEST	0xE9					/**< Mouse status request command. */
#define WRITE_MOUSE_BYTE		0xD4					/**< Write mouse byte command. */

/**
 * Mouse Buttons.
 */
#define MOUSE_RB				0x09					/**< Mouse right button. */
#define MOUSE_LB				0x0A					/**< Mouse left button. */
#define MOUSE_MB				0x0C					/**< Mouse middle button. */
#define MOUSE_LB_RB				(MOUSE_RB | MOUSE_LB) 	/**< Mouse right and left button. */

/**
 * Mouse LED's.
 */
#define SCROLL_LCK		0x01	// 001					/**< Scroll lock LED. */
#define NUM_LCK			0x02	// 010					/**< Num lock LED. */
#define CAPS_LCK		0x04	// 100					/**< Caps lock LED. */

#endif /* I8042_H_ */
