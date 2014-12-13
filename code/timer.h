#ifndef __TIMER_H
#define __TIMER_H

#include "libraries.h"

/** @defgroup timer timer
 * @{
 *
 * Functions related to the timer 0.
 */

/**
 * @brief Sets timer to square mode.
 *
 * @param timer Timer to set.
 * @param freq Frequency of the timer.
 *
 * @return 0 if success, -1 otherwise.
 */
int timer_set_square(unsigned long timer, unsigned long freq);

/**
 * @brief Subscribes timer.
 *
 * @param timer0_id ID of the timer.
 *
 * @return 0 if success, -1 otherwise.
 */
int timer_subscribe_int(const unsigned int timer0_id);

/**
 * @brief Unsubscribes timer.
 *
 * @param timer0_id ID of the timer to unsubscribe.
 *
 * @return 0 if success, -1 otherwise.
 */
int timer_unsubscribe_int(const unsigned int timer0_id);

#endif /* __TIMER_H */
