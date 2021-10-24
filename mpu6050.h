#ifndef MPU6050_H
#define MPU6050_H

#include "vector.h"

#define MPU6050_ADDR                   0x68 // 0x69 when AD0 pin to Vcc
#define MPU6050_REG_PWR_MGMT_1         0x6B // Power Management 1
#define MPU6050_REG_ACCEL_CONFIG       0x1C // Accelerometer config
#define MPU6050_REG_GYRO_CONFIG        0x1B // Gyro config
#define MPU6050_REG_TEMP_OUT_H         0x41 // Temperature higher byte
#define MPU6050_REG_TEMP_OUT_L         0x42 // Temperature lower byte
#define MPU6050_REG_ACCEL_XOUT_H       0x3B // Accelerometer data; first of six bytes (2 each for X, Y and Z)
#define MPU6050_REG_GYRO_XOUT_H        0x43 // Gyroscope data; first of six bytes (2 each for X, Y and Z)

#define RAD_TO_DEG 57.295779513082320876798154814105 // 180 / PI

void mpu6050_init(void);

float mpu6050_read_temperature(void);

Vec3_f mpu6050_read_accelerometer_xyz(void);
Vec3_f mpu6050_calculate_accelerometer_error_xyz(void);

Vec2_f mpu6050_read_accelerometer_xy(void);
Vec2_f mpu6050_calculate_accelerometer_error_xy(void);

Vec2_f mpu6050_read_gyroscope_xy(void);
Vec2_f mpu6050_calculate_gyroscope_error_xy(void);

#endif
