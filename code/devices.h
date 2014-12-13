#ifndef DEVICES_H_
#define DEVICES_H_

/**
 * @brief Handles the initialization of all the devices.
 *
 * Subscribes timer, keyboard and mouse.
 *
 * @return Success value of all the devices.
 */
int devices_startup();

/**
 * @brief Handles the unsubscribing of all the devices.
 *
 * Unsubscribes timer, keyboard and mouse.
 *
 * @return Success value of all the devices.
 */
int devices_exit();

#endif
