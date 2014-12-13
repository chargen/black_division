/*
 * main.c
 *
 * Created 28/11/2013
 * Author: ei12054
 */

#include "libraries.h"
#include "devices.h"

const long int vg_init_mode = 0x105;

static void print_usage(char *argv[]);
static int proc_args(int argc, char *argv[]);

int main(int argc, char **argv) {

	sef_startup();

	int state = 0;
	int debugmode = 0;

	srand(time(NULL));

	if ( argc == 1 ) {
		print_usage(argv);
		return 0;
	} else {

		if((debugmode = proc_args(argc, argv)) > -1) {

			/**
			 * Called initially because of the unknown issue which makes colors less bright on the first time.
			 */
			vg_init(vg_init_mode);
			vg_exit();

			if(!debugmode) {
				vg_init(vg_init_mode);
			}

			state = devices_startup(); /* Start all devices. Any errors will be stored in state and handled/reported below. */

			if(state == 0) { /* everything OK */
				timer_set_square(0,60);
				clean_out_buf();
				gameLoop(debugmode);
				if(!debugmode) {
					vg_exit();
				}
			}
			else {
				if(!debugmode) {
					vg_exit();
				}
				if(BIT0(state)) {
					printf("\n ------> Error subscribing the timer! Exiting...\n");
				}
				if(BIT1(state)) {
					printf("\n ------> Error subscribing the keyboard! Exiting...\n");
				}
				if(BIT2(state)) {
					printf("\n ------> Error subscribing the mouse! Exiting...\n");
				}
				if(BIT3(state)) {
					printf("\n ------> Error enabling mouse stream! Exiting...\n");
				}
			}

			printf("\nCleaning out buffer...\n");
			clean_out_buf();

			printf("\nstate 1: %d\n", state);
			state = 0;
			state = devices_exit();
			printf("\nstate 2: %d\n", state);

			if(!state) {
				printf("\n ------> Sucessfull unsubscribes! Exiting...\n");
				return 0;
			}
			else {
				if(BIT0(state)) {
					printf("\n ------> Error unsubscribing the timer! Exiting...\n");
				}
				if(BIT1(state)) {
					printf("\n ------> Error unsubscribing the keyboard! Exiting...\n");
				}
				if(BIT2(state)) {
					printf("\n ------> Error unsubscribing the mouse! Exiting...\n");
				}
				if(BIT3(state)) {
					printf("\n ------> Error disabling mouse stream! Exiting...\n");
				}
				return -1;
			}

		}
		else {
			printf("\n Not correct arguments for usage! \n");
		}

	}

	printf("\n--> OVER <--\n");

	return 0;
}

static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"game\" \n"
	 "\t service run %s -args \"debug\" \n",
	 argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	unsigned short duration;
	if (strncmp(argv[1], "game", strlen("game")) == 0) {
		if( argc != 2 ) {
			printf("-> test: wrong no of arguments to run the game \n");
			return -1;
		}

		printf("-> Run the game! \n");
		return 0;
	}
	else if (strncmp(argv[1], "debug", strlen("debug")) == 0) {
		if( argc != 2 ) {
			printf("-> test: wrong no of arguments to run the game in debugmode \n");
			return -1;
		}

		printf("->  Run the game in debugmode! \n");
		return 1;
	}

	return -1;
}
