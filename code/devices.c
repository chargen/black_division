#include "devices.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "speaker.h"
#include "i8254.h"
#include "i8042.h"

unsigned int TIMER_ID = 0;
unsigned int KEYBOARD_ID = 1;
unsigned int MOUSE_ID = 2;
unsigned int SPEAKER_ID = 3;
unsigned int RTC_ID = 4;

int devices_startup() {

	int return_value = 0;

	if(timer_subscribe_int(TIMER_ID) == -1) {
		return_value = return_value | 1;
	}

	if(keyboard_subscribe_int(KEYBOARD_ID) == -1) {
		return_value = return_value | 2;
	}

	if(mouse_subscribe_int(MOUSE_ID) == -1) {
		return_value = return_value | 4;
	}

	if(send_cmd_to_mouse(MOUSE_DISABLE_STREAM) == -1) {
		return_value = return_value | 8;
	}

	if(send_cmd_to_mouse(MOUSE_ENABLE_STREAM) == -1) {
		return_value = return_value | 8;
	}

	return return_value;

}

int devices_exit() {

	int return_value = 0;

	if(timer_unsubscribe_int(TIMER_ID) == -1) {
		return_value = return_value | 1;
	}

	if(keyboard_unsubscribe_int(KEYBOARD_ID) == -1) {
		return_value = return_value | 2;
	}

	if(mouse_unsubscribe_int(MOUSE_ID) == -1) {
		return_value = return_value | 4;
	}

	return return_value;
}
