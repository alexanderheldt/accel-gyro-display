#ifndef SSD1306_H
#define SSD1306_H

#include "stdint.h"
#include "entity.h"

#define SSD1306_ADDR                0x3C // 0111100

#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_DEACTIVATE_SCROLL   0x2E
#define SSD1306_DISPLAYON           0xAF

void ssd1306_init(void);

void ssd1306_clear(void);
void ssd1306_render(Entity* entities, uint8_t size);
void ssd1306_switchRenderFrame(void);
void ssd1306_switchDisplayFrame(void);
void ssd1306_switchFrame(void);

uint8_t ssd1306_calculatePageFor(uint8_t y);
void ssd1306_setCursor(uint8_t x, uint8_t y);
bool ssd1306_inBounds(uint8_t x, uint8_t y);
void ssd1306_write8(uint8_t pixels);
void ssd1306_command(unsigned char c);

#endif
