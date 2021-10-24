#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "millis_config.h"
#include "millis.h"

#include "vector.h"
#include "uart.h"
#include "i2c.h"
#include "ssd1306.h"
#include "mpu6050.h"

#include "animation.h"
#include "sand.h"

#define TIME_STEP 1 / 30.f // 30 fps

void update_acceleration(Vec2_f* a) {
  static uint8_t sampleRateMs = 50;

  static uint32_t lastMs;
  uint32_t now = millis();

  if (now - lastMs >= sampleRateMs) {
    lastMs = now;

    Vec2_f raw = mpu6050_read_accelerometer_xy();

    // Compensate for errors
    a->x = raw.x + 0.05f;
    a->y = (raw.y - 0.08f) * -1; // Flip y-axis to accomodate for screen rotation
  }
}

void render(Entity* entities, uint8_t size) {
  ssd1306_clear();
  ssd1306_render(entities, size);
  ssd1306_switchFrame();
}

int main() {
  // set pin 2 as output
  DDRB |= (1 << DDB3);

  sei(); // enable interrupts for millis
  millis_init();
  uart_init(); // used to print fps
  i2c_init();
  mpu6050_init();

  ssd1306_init();
  ssd1306_clear();
  ssd1306_switchRenderFrame();

  uint8_t size;
  Entity* pattern = sand_init(&size);

  static Vec2_f acceleration = Vec2_f{0.f, 0.f};

  static uint32_t lastFrameAt = 0;
  static float accumulatorSeconds = 0.f;
  static uint8_t frameCount = 0;

  while(1) {
    update_acceleration(&acceleration);

    uint32_t now = millis();
    accumulatorSeconds += (now - lastFrameAt) / 1000.0;

    while (accumulatorSeconds > TIME_STEP) {
      sand_update(&acceleration, TIME_STEP, pattern, size);
      accumulatorSeconds -= TIME_STEP;
    }

    render(pattern, size);
    lastFrameAt = now;

    animation_fps(&now);

    PORTB ^= (1 << PB3);
  }

  return 0;
}
