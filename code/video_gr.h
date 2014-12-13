#ifndef VIDEO_GR_H_
#define VIDEO_GR_H_

#include "libraries.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions related to the graphics and video buffer.
 */

/**
 * @brief Initialize video graphics mode.
 *
 * @param mode mode to initialize
 */
int vg_init(unsigned short mode);
/**
 * @brief Exit video graphics mode.
 */
int vg_exit();
/**
 * @brief Swap two values.
 *
 * @param a value a
 * @param b value b
 */
void swap(unsigned int *a, unsigned int *b);
/**
 * @brief Fill the video buffer with a color.
 *
 * @param color color to fill with
 */
int vg_fill(unsigned int color);
/**
 * @brief Set a pixel in the position (x,y).
 *
 * @param x x coordinate
 * @param y y coordinate
 * @param color color of the pixel
 */
int vg_set_pixel(unsigned int x, unsigned int y, unsigned int color);
/**
 * @brief Get the color of the pixel in the position (x,y).
 *
 * @param x x coordinate
 * @param y y coordinate
 */
unsigned long vg_get_pixel(unsigned int x, unsigned int y);
/**
 * @brief Transpose the video buffer to the video memory.
 */
int vg_draw_buffer_to_mem();
/**
 * @brief Draw a number in the video buffer.
 *
 * @param xi number's x coordinate (top)
 * @param yi number's y coordinate (left)
 * @param color color of the number
 * @param number number to be written
 * @param size size of the number
 */
int vg_draw_number(const unsigned int xi, const unsigned int yi, const unsigned int color, unsigned long number, const unsigned int size);
/**
 * @brief Draw a special char in the video buffer ("(",")","'").
 *
 * @param xi char's x coordinate (top)
 * @param yi char's y coordinate (left)
 * @param color color of the char
 * @param symbol char to be written
 * @param size size of the char
 */
int vg_draw_special(const unsigned int xi, const unsigned int yi, const unsigned int color, const char symbol, const unsigned int size);
/**
 * @brief Draw a char in the video buffer.
 *
 * @param xi char's x coordinate (top)
 * @param yi char's y coordinate (left)
 * @param color color of the char
 * @param symbol char to be written
 * @param size size of the char
 */
int vg_draw_char(const unsigned int xi, const unsigned int yi, const unsigned int color, const char symbol, const unsigned int size);
/**
 * @brief Draw a line in the video buffer.
 *
 * @param xi line's x initial coordinate
 * @param yi line's y initial coordinate
 * @param xf line's x final coordinate
 * @param yf line's y final coordinate
 * @param color color of the line
 */
int vg_draw_line(unsigned int xi, unsigned int yi, unsigned int xf, unsigned int yf, unsigned int color);
/**
 * @brief Draw a square in the video buffer.
 *
 * @param xi square's x initial coordinate
 * @param yi square's y initial coordinate
 * @param size size of the square
 * @param color color of the square
 */
int vg_draw_square(unsigned int xi, unsigned int yi, unsigned int size, unsigned int color);
/**
 * @brief Draw a rectangle in the video buffer.
 *
 * @param xi rectangle's x initial coordinate
 * @param yi rectangle's y initial coordinate
 * @param xf rectangle's x final coordinate
 * @param yf rectangle's y final coordinate
 * @param color color of the rectangle
 */
int vg_draw_rectangle(unsigned int xi, unsigned int yi, unsigned int xf, unsigned int yf, unsigned int color);
/**
 * @brief Draw a game square in the video buffer.
 *
 * @param xi square's x initial coordinate
 * @param yi square's y initial coordinate
 * @param size size of the square
 * @param color[] array with the colors of each of the 8 divisions
 */
int vg_draw_death_square(unsigned int xi, unsigned int yi, unsigned int size, unsigned int color[]);
/**
 * @brief Fill a section of the video buffer with a color.
 *
 * @param xi section's x initial coordinate
 * @param yi section's y initial coordinate
 * @param xf section's x final coordinate
 * @param yf section's y final coordinate
 * @param color color to fill
 */
int vg_fill_section(unsigned int xi, unsigned int yi, unsigned int xf, unsigned int yf, unsigned int color);

#endif /* VIDEO_GR_H_ */
