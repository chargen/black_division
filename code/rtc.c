/*
 * rtc.c
 *
 * Created 28/11/2013
 * Author: ei12054
 */

#include "rtc.h"

RTC rtc;

const int BCDlength = 1;	/**< @brief Used int parseBCD. */
const int millenium = 2;	/**< @brief Current millenium (2000). */

int parseBCD(unsigned long bcd, unsigned int length) {

	unsigned int result = 0;

	while (length--) {

	        result = result * 100 + (bcd >> 4) * 10 + (bcd & 0xf);
	        ++bcd;

	}

	return result;

}

void wait_valid_rtc(void) {

	unsigned long regA = 0;

	do {

		sys_outb(RTC_ADDR_REG, RTC_REG_A);
		sys_inb(RTC_DATA_REG, &regA);

	} while ( regA & UIP(regA));

}

char * rtc_get_week_day(RTC game_rtc) {

	char * str = malloc(4);

	switch(parseBCD(game_rtc.data[RTC_DAY_WEEK], BCDlength)) {
	case 1:
		strcpy(str, "sun.");
		break;
	case 2:
		strcpy(str, "mon.");
		break;
	case 3:
		strcpy(str, "tue.");
		break;
	case 4:
		strcpy(str, "wed.");
		break;
	case 5:
		strcpy(str, "thu.");
		break;
	case 6:
		strcpy(str, "fri.");
		break;
	case 7:
		strcpy(str, "sat.");
		break;
	default:
		strcpy(str, "err.");
		break;
	}

	return str;

}

char * rtc_get_month(RTC game_rtc) {
	char * str = malloc(4);
	switch(parseBCD(game_rtc.data[RTC_MONTH], BCDlength)) {
	case 1:
		strcpy(str, "jan.");
		break;
	case 2:
		strcpy(str, "fev.");
		break;
	case 3:
		strcpy(str, "mar.");
		break;
	case 4:
		strcpy(str, "apr.");
		break;
	case 5:
		strcpy(str, "may.");
		break;
	case 6:
		strcpy(str, "jun.");
		break;
	case 7:
		strcpy(str, "jul.");
		break;
	case 8:
		strcpy(str, "aug.");
		break;
	case 9:
		strcpy(str, "sep.");
		break;
	case 10:
		strcpy(str, "out.");
		break;
	case 11:
		strcpy(str, "nov.");
		break;
	case 12:
		strcpy(str, "dec.");
		break;
	default:
		strcpy(str, "err.");
		break;
	}
	return str;
}

int rtc_get_month_day(RTC game_rtc) {
	return parseBCD(game_rtc.data[RTC_DAY_MONTH], BCDlength);
}

int rtc_get_year(RTC game_rtc) {
	int year = parseBCD(game_rtc.data[RTC_YEAR], BCDlength);
	return (millenium*1000 + year);
}

int rtc_get_hours(RTC game_rtc) {
	return parseBCD(game_rtc.data[RTC_HOUR], BCDlength);
}

int rtc_get_minutes(RTC game_rtc) {
	return parseBCD(game_rtc.data[RTC_MIN], BCDlength);
}

int rtc_get_seconds(RTC game_rtc) {
	return parseBCD(game_rtc.data[RTC_SEC], BCDlength);
}

RTC rtc_get_date(void) {
	unsigned int i,index;
	int length = 1;
	unsigned long register_data[9];

	wait_valid_rtc();

	for(i = 0; i < 10; i++) {
		sys_outb(RTC_ADDR_REG, i);
		sys_inb(RTC_DATA_REG, &rtc.data[index]);
		index++;
		if(i < 6) i++;
	}

	return rtc;
}

