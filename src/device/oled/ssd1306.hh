#pragma once

#include "ssd1306_conf.hh"

#include <stdint.h>

#ifdef SSD1306_X_OFFSET
#define SSD1306_X_OFFSET_LOWER (SSD1306_X_OFFSET & 0x0F)
#define SSD1306_X_OFFSET_UPPER ((SSD1306_X_OFFSET >> 4) & 0x07)
#else
#define SSD1306_X_OFFSET_LOWER 0
#define SSD1306_X_OFFSET_UPPER 0
#endif

// SSD1306 OLED height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT 64
#endif

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH 128
#endif

#ifndef SSD1306_BUFFER_SIZE
#define SSD1306_BUFFER_SIZE SSD1306_WIDTH* SSD1306_HEIGHT / 8
#endif

namespace oled {

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01 // Pixel is set. Color depends on OLED
} Color;

typedef enum {
    SSD1306_OK = 0x00,
    SSD1306_ERR = 0x01 // Generic error.
} Status;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Initialized;
    uint8_t DisplayOn;
} Screen;

typedef struct {
    uint8_t x;
    uint8_t y;
} Vertex;

/** Font */
typedef struct {
    const uint8_t width; /**< Font width in pixels */
    const uint8_t height; /**< Font height in pixels */
    const uint16_t* const data; /**< Pointer to font data array */
    const uint8_t* const char_width; /**< Proportional character width in pixels (NULL for monospaced) */
} Font;

// Procedure definitions
void init(void);
void fill(Color color);
void update(void);
void draw_pixel(uint8_t x, uint8_t y, Color color);
char write_char(char ch, Font font, Color color);
char write_string(char* str, Font font, Color color);
void set_cursor(uint8_t x, uint8_t y);
void draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
void draw_arc(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, Color color);
void draw_arc_with_radius_line(uint8_t x, uint8_t y, uint8_t radius, uint16_t start_angle, uint16_t sweep, Color color);
void draw_circle(uint8_t par_x, uint8_t par_y, uint8_t par_r, Color color);
void fill_circle(uint8_t par_x, uint8_t par_y, uint8_t par_r, Color par_color);
void poly_line(const Vertex* par_vertex, uint16_t par_size, Color color);
void draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);
void fill_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, Color color);

/**
 * @brief Invert color of pixels in rectangle (include border)
 *
 * @param x1 X Coordinate of top left corner
 * @param y1 Y Coordinate of top left corner
 * @param x2 X Coordinate of bottom right corner
 * @param y2 Y Coordinate of bottom right corner
 * @return SSD1306_Error_t status
 */
Status invert_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void draw_bitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, Color color);

/**
 * @brief Sets the contrast of the display.
 * @param[in] value contrast to set.
 * @note Contrast increases as the value increases.
 * @note RESET = 7Fh.
 */
void set_contrast(const uint8_t value);

/**
 * @brief Set Display ON/OFF.
 * @param[in] on 0 for OFF, any for ON.
 */
void set_displayOn(const uint8_t on);

/**
 * @brief Reads DisplayOn state.
 * @return  0: OFF.
 *          1: ON.
 */
uint8_t get_display_on();

// Low-level procedures
Status fill_buffer(uint8_t* buf, uint32_t len);
}
