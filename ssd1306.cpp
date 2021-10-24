#include <stdlib.h>
#include <stdio.h>

#include "i2c.h"
#include "uart.h"
#include "ssd1306.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32

uint8_t renderingFrame = 0xB0;
uint8_t drawingFrame = 0x40;

void ssd1306_init() {
    ssd1306_command(SSD1306_DISPLAYOFF);

    ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);
    ssd1306_command(0x80);

    ssd1306_command(SSD1306_SETMULTIPLEX);
    ssd1306_command(SSD1306_HEIGHT - 1);

    ssd1306_command(SSD1306_SETDISPLAYOFFSET);
    ssd1306_command(0x00);

    ssd1306_command(SSD1306_SETSTARTLINE | 0x00);

    ssd1306_command(SSD1306_CHARGEPUMP);
    ssd1306_command(0x14);

    ssd1306_command(SSD1306_MEMORYMODE);
    ssd1306_command(0x00);

    uint8_t rotation = 0 & 0x01; // horizontal flip
    ssd1306_command(SSD1306_SEGREMAP | rotation);
    //ssd1306_command(SSD1306_SEGREMAP | 0x01);

    //ssd1306_command(0xC0 | (rotation << 3));
    ssd1306_command(0xC0 | (0 << 3));
    //ssd1306_command(SSD1306_COMSCANDEC);

    ssd1306_command(SSD1306_SETCOMPINS);
    ssd1306_command(0x02);

    ssd1306_command(SSD1306_SETCONTRAST);
    ssd1306_command(0xAF);

    ssd1306_command(SSD1306_SETPRECHARGE);
    ssd1306_command(0xF1);

    ssd1306_command(SSD1306_SETVCOMDETECT);
    ssd1306_command(0x40);

    ssd1306_command(SSD1306_DISPLAYALLON_RESUME);
    ssd1306_command(SSD1306_NORMALDISPLAY);
    ssd1306_command(SSD1306_DEACTIVATE_SCROLL);
    ssd1306_command(SSD1306_DISPLAYON);
}

void ssd1306_clear(void) {
  ssd1306_setCursor(0, 0);
  i2c_start(SSD1306_ADDR, 0);
  i2c_write(0x40);

  for (uint16_t i = 0; i < ((SSD1306_HEIGHT * SSD1306_WIDTH) / 8); i++) {
      i2c_write(0x00);
  }

  i2c_stop();
}

void ssd1306_render(Entity* entities, uint8_t size) {
  // for e in entities:
  // dy = ...;
  // pixels = (1 << dy):
  // for other_e in entities:
  //    if other_e.x == e.x && calculatePageFor(other_e.y) == calculatePageFor(e.y):
  //      pixels |= (1 << other_e_dy)

  for (uint8_t i = 0; i < size; i++) {
    Entity e = entities[i];

    // Not in bounds; should not be rendered
    if (!ssd1306_inBounds(e.position.x, e.position.y)) continue;

    uint8_t e_page = ssd1306_calculatePageFor(e.position.y);
    uint8_t e_dy = e.position.y - (8 * e_page);
    uint8_t pixels = 0x00 | (1 << e_dy);

    for (uint8_t j = 0; j < size; j++) {
      Entity e2 = entities[j];

      if (&e == &e2) continue;

      // If the elements are not on the same x
      // then `e2` will not affect the pixels below/above `e`
      if (e.position.x != e2.position.x) continue;

      // If the elements are not on the same page
      // then `e2` will not affect the pixels below/above `e`
      uint8_t e2_page = ssd1306_calculatePageFor(e2.position.y);
      if (e_page != e2_page) continue;

      // Not in bounds; should not be rendered
      if (!ssd1306_inBounds(e2.position.x, e2.position.y)) continue;

      uint8_t e2_dy = e2.position.y - (8 * e2_page);
      pixels |= (1 << e2_dy);
    }

    ssd1306_setCursor(e.position.x, e_page);
    ssd1306_write8(pixels);
  }
}

void ssd1306_switchRenderFrame(void) {
  renderingFrame ^= 0x04;
}

void ssd1306_switchDisplayFrame(void) {
  drawingFrame ^= 0x20;
  ssd1306_command(drawingFrame);
}

void ssd1306_switchFrame(void) {
  ssd1306_switchRenderFrame();
  ssd1306_switchDisplayFrame();
}

uint8_t ssd1306_calculatePageFor(uint8_t y) {
  if (y < 8) return 0;
  if (y > 7 && y < 16) return 1;
  if (y > 15 && y < 24) return 2;

  // y > 23
  return 3;
}

void ssd1306_setCursor(uint8_t x, uint8_t y) {
    ssd1306_command(renderingFrame | (y & 0x07));
    ssd1306_command(0x10 | ((x & 0xF0) >> 4));
    ssd1306_command(x & 0x0F);
}

bool ssd1306_inBounds(uint8_t x, uint8_t y) {
  return x >= 0 && x < SSD1306_WIDTH && y >= 0 && y < SSD1306_HEIGHT;
}

void ssd1306_write8(uint8_t pixels) {
    i2c_start(SSD1306_ADDR, 0);
    i2c_write(0x40);
    i2c_write(pixels);
    i2c_stop();
}

void ssd1306_command(uint8_t c) {
    i2c_start(SSD1306_ADDR, 0);
    i2c_write(0x00);
    i2c_write(c);
    i2c_stop();
}
