#include <stdint.h>
#include "i2c.h"
#include "mpu6050.h"

void mpu6050_init() {
  i2c_write_register8(MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, 0b00000000); // reset
  i2c_write_register8(MPU6050_ADDR, MPU6050_REG_ACCEL_CONFIG, 0b00000000); // set full scale range to +- 2g
  i2c_write_register8(MPU6050_ADDR, MPU6050_REG_GYRO_CONFIG, 0b00000000); // set full scale range to +- 250deg/s
}

float mpu6050_read_temperature(void) {
  int16_t T = i2c_read_register16(MPU6050_ADDR, MPU6050_REG_TEMP_OUT_H);
  return (float)T/340 + 36.53;
}

Vec3_f mpu6050_read_accelerometer_xyz(void) {
    i2c_start(MPU6050_ADDR, 0);
    i2c_write(MPU6050_REG_ACCEL_XOUT_H);
    i2c_restart(MPU6050_ADDR, 6);

    uint8_t xha = i2c_read();
    uint8_t xla = i2c_read();
    uint8_t yha = i2c_read();
    uint8_t yla = i2c_read();
    uint8_t zha = i2c_read();
    uint8_t zla = i2c_read();

    i2c_stop();

    Vec3_f v;
    // divide by 16384 because of +-2g full scale range
    v.x = (xha << 8 | xla) / 16384.0;
    v.y = (yha << 8 | yla) / 16384.0;
    v.z = (zha << 8 | zla) / 16384.0;

    return v;
}

Vec3_f mpu6050_calculate_accelerometer_error_xyz() {
  uint8_t total_samples = 200;
  uint8_t sample = 0;

  float sum_error_x, sum_error_y, sum_error_z;

  while (sample < total_samples) {
    Vec3_f a = mpu6050_read_accelerometer_xyz();

    sum_error_x += a.x;
    sum_error_y += a.y;
    sum_error_z += a.z;

    sample++;
  }

  Vec3_f error;
  error.x = (float) sum_error_x / total_samples;
  error.y = (float) sum_error_y / total_samples;
  error.z = (float) sum_error_z / total_samples;

  return error;
}

Vec2_f mpu6050_read_accelerometer_xy(void) {
    i2c_start(MPU6050_ADDR, 0);
    i2c_write(MPU6050_REG_ACCEL_XOUT_H);
    i2c_restart(MPU6050_ADDR, 4);

    uint8_t xha = i2c_read();
    uint8_t xla = i2c_read();
    uint8_t yha = i2c_read();
    uint8_t yla = i2c_read();

    i2c_stop();

    Vec2_f v;
    // divide by 16384 because of +-2g full scale range
    v.x = (xha << 8 | xla) / 16384.0;
    v.y = (yha << 8 | yla) / 16384.0;

    return v;
}

Vec2_f mpu6050_calculate_accelerometer_error_xy() {
  uint8_t total_samples = 200;
  uint8_t sample = 0;

  float sum_error_x, sum_error_y;

  while (sample < total_samples) {
    Vec2_f a = mpu6050_read_accelerometer_xy();

    sum_error_x += a.x;
    sum_error_y += a.y;

    sample++;
  }

  Vec2_f error;
  error.x = (float) sum_error_x / total_samples;
  error.y = (float) sum_error_y / total_samples;

  return error;
}

Vec2_f mpu6050_read_gyroscope_xy(void) {
  i2c_start(MPU6050_ADDR, 0);
  i2c_write(MPU6050_REG_GYRO_XOUT_H);
  i2c_restart(MPU6050_ADDR, 4);

  uint8_t xha = i2c_read();
  uint8_t xla = i2c_read();
  uint8_t yha = i2c_read();
  uint8_t yla = i2c_read();

  i2c_stop();

  Vec2_f v;
  // divide by 131 because of +-250 deg/s full scale range
  v.x = (xha << 8 | xla) / 131.0;
  v.y = (yha << 8 | yla) / 131.0;

  return v;
}

Vec2_f mpu6050_calculate_gyroscope_error_xy(void) {
  uint8_t total_samples = 200;
  uint8_t sample = 0;

  float sum_error_x, sum_error_y;

  while (sample < total_samples) {
    Vec2_f g = mpu6050_read_gyroscope_xy();

    sum_error_x += g.x;
    sum_error_y += g.y;

    sample++;
  }

  Vec2_f error;
  error.x = (float) sum_error_x / total_samples;
  error.y = (float) sum_error_y / total_samples;

  return error;
}
