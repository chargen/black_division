#ifndef RTC_H_
#define RTC_H_

#include "libraries.h"

/** @defgroup rtc rtc
 * @{
 *
 * Functions related to low memory (first 1 MB of physical memory), required for BIOS
 */

#define RTC_REG_A	10						/**< @brief RTC Register A. */
#define RTC_REG_B	11						/**< @brief RTC Register B. */
#define RTC_REG_C	12						/**< @brief RTC Register C. */
#define RTC_REG_D	13						/**< @brief RTC Register D. */

#define RTC_SEC			0					/**< @brief Seconds. */
#define RTC_MIN			1					/**< @brief Minutes. */
#define RTC_HOUR		2					/**< @brief Hours. */
#define RTC_DAY_WEEK	3					/**< @brief Day of the Week. */
#define RTC_DAY_MONTH	4					/**< @brief Day of the Month. */
#define RTC_MONTH		5					/**< @brief Month. */
#define RTC_YEAR		6					/**< @brief Year. */

#define UIP(b) ( (b) >> 7)
#define DM(b) (( (b) << 5) >> 7)
#define HOUR_TYPE(b) (( (b) << 6) >> 7)
#define DSE(b) (( (b) << 7) >> 7)
#define IRQF(b) ( (b) >> 7)
#define PF(b) (( (b) << 1) >> 7)
#define AF(b) (( (b) << 2) >> 7)
#define UF(b) (( (b) << 3) >> 7)
#define VRT(b) ( (b) >> 7)

#define RTC_ADDR_REG	0x70	/**< @brief RTC Address Register. */
#define RTC_DATA_REG	0x71	/**< @brief RTC Data Register. */

/** @name  RTC Data Struct*/
/**@{
 *
 * Struct that keeps info regarding the mapping of physical memory to virtual memory
 */
typedef struct {
	unsigned long data[10];	/**< @brief rtc data array. */
} RTC;
/** @} end of Memory Map Struct */

/**
 * @brief Parses Binary Coded Decimal.
 *
 * @param bcd BCD to parse.
 * @param length Length of the BCD to parse.
 *
 * @return Parsed BCD.
 */
int parseBCD(unsigned long bcd, unsigned int length);

/**
 * @brief Waits for the RTC to be valid.
 */
void wait_valid_rtc(void);

/**
 * @brief Gets week of the day from RTC.
 *
 * @param game_rtc RTC from wich we get the week day.
 *
 * @return String with the day of the week.
 */
char * rtc_get_week_day(RTC game_rtc);

/**
 * @brief Gets month from RTC.
 *
 * @param game_rtc RTC from wich we get the month.
 *
 * @return String with the month.
 */
char * rtc_get_month(RTC game_rtc);

/**
 * @brief Gets day of the month from RTC.
 *
 * @param game_rtc RTC from wich we get the day of the month.
 *
 * @return Day of the month.
 */
int rtc_get_month_day(RTC game_rtc);

/**
 * @brief Gets year from RTC.
 *
 * @param game_rtc RTC from wich we get the year.
 *
 * @return Year.
 */
int rtc_get_year(RTC game_rtc);

/**
 * @brief Gets hour from RTC.
 *
 * @param game_rtc RTC from wich we get the hour.
 *
 * @return Hour.
 */
int rtc_get_hours(RTC game_rtc);

/**
 * @brief Gets minutes from RTC.
 *
 * @param game_rtc RTC from wich we get the minutes.
 *
 * @return Minutes.
 */
int rtc_get_minutes(RTC game_rtc);

/**
 * @brief Gets seconds from RTC.
 *
 * @param game_rtc RTC from wich we get the seconds.
 *
 * @return Seconds.
 */
int rtc_get_seconds(RTC game_rtc);

/**
 * @brief Gets complete data to RTC.
 *
 * @return  RTC Data Struct with the data.
 */
RTC rtc_get_date(void);

#endif /* RTC_H_ */
