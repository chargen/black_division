#ifndef MOUSE_H_
#define MOUSE_H_

#include "libraries.h"

/** @defgroup mouse mouse
 * @{
 *
 * Functions related to the mouse.
 */

#define MOUSE_STATUS_OK			0xFA	/**< @brief Mouse status OK code. */
#define MOUSE_STATUS_REQUEST	0xE9	/**< @brief Mouse status request command. */

#define MASK 0x0B						/**< @brief Mask. */

#define LB(b) 		BIT0(b) 			/**< @brief Left Button Bit. */
#define RB(b) 		BIT1(b) 			/**< @brief Right Button Bit. */
#define MB(b) 		BIT2(b) 			/**< @brief Middle Button Bit. */
#define XSIGN(b) 	BIT4(b) 			/**< @brief X sign Bit (movement). */
#define YSIGN(b) 	BIT5(b) 			/**< @brief Y sign Bit (movement). */
#define XOV(b)		BIT6(b) 			/**< @brief X Overflow Bit (movement). */
#define YOV(b)		BIT7(b) 			/**< @brief Y Overflow Bit (movement). */

/**
 * @brief Subscribes mouse.
 *
 * @param mouse_id Mouse ID.
 *
 * @return 0 if success, -1 otherwise.
 */
int mouse_subscribe_int(unsigned int mouse_id);

/**
 * @brief Unsubscribes mouse.
 *
 * @param mouse_id Mouse ID.
 *
 * @return 0 if success, -1 otherwise.
 */
int mouse_unsubscribe_int(unsigned int mouse_id);

/**
 * @brief Waits for the mouse input buffer to be empty.
 *
 * @return 1 if buffer is empty.
 */
int in_buf_empty();

/**
 * @brief Cleans output buffer of the mouse.
 *
 * @return 0 when output buffer is empty.
 */
int clean_out_buf();

/**
 * @brief Sends a command to the mouse.
 *
 * @param cmd Command to send to the mouse.
 *
 * @return 0 if success, -1 otherwise.
 */
int send_cmd_to_mouse(unsigned char cmd);

/**
 * @brief Receive packets from the mouse.
 *
 * @return Packets (data) sent by the mouse device.
 */
int mouse_receive_packet();

#endif /* MOUSE_H_ */
