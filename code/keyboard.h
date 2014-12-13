#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "libraries.h"

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions related to the keyboard.
 */

#define OBF 			BIT(0)		/**< @brief Output Buffer Full. */
#define IBF				BIT(1)		/**< @brief Input Buffer Full. */
#define TO_ERR			BIT(6)		/**< @brief Timeout Error. */
#define PAR_ERR			BIT(7)		/**< @brief Parity Error. */

#define KBD_RESET		0xFF		/**< @brief Reset keyboard command. */
#define KBD_SET_LEDS_0	0x00		/**< @brief Turn off all leds command. */
#define KBD_SWITCH_LEDS	0xED		/**< @brief Switch leds command. */

#define RESEND_ERR 		0xFE		/**< @brief Resend Error. */
#define ERROR			0xFC		/**< @brief Error. */

#define TWO_BYTE_CODE	0xE0		/**< @brief Two byte char code. */
#define BREAKCODE		0x80		/**< @brief Breakcode. */

/**
 * @brief Sets the LED's of the keyboard.
 *
 * @param number of LED to be turned on/off.
 *
 * @return 0 if success, -1 otherwise.
 */
int keyboard_set_leds(unsigned short int number);

/**
 * @brief Subscribes keyboard.
 *
 * @param keyboard_id ID of the keyboard.
 *
 * @return 0 if success, -1 otherwise.
 */
int keyboard_subscribe_int(const unsigned int keyboard_id);

/**
 * @brief Unsubscribes keyboard.
 *
 * @param keyboard_id ID of the keyboard.
 *
 * @return 0 if success, -1 otherwise.
 */
int keyboard_unsubscribe_int(const unsigned int keyboard_id);

/**
 * @brief Scans the keyboard input buffer.
 *
 * @return Data scanned (Char code).
 * @return -1 if an error occurs.
 */
unsigned long keyboard_scan();

#endif /* KEYBOARD_H_ */
