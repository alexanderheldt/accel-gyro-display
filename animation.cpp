#include <stdio.h>
#include "millis_config.h"
#include "millis.h"
#include "uart.h"
#include "animation.h"

#define SAMPLE_RATE_MS 5000

void animation_fps(uint32_t* now) {
  static uint32_t lastFPSsample = 0;
  static uint8_t frameCount;

  frameCount++;

  if (*now - lastFPSsample >= SAMPLE_RATE_MS) {
    uint8_t fps = frameCount / (SAMPLE_RATE_MS / 1000);

    frameCount = 0;
    lastFPSsample = *now;

    char buf[32];
    sprintf(buf, "fps: %d\n", fps);
    uart_tx_str(buf);
  }
}
