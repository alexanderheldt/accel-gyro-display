/* Adapted from Tiny I2C

   David Johnson-Davies - www.technoblogy.com - 28th May 2019

   CC BY 4.0
   Licensed under a Creative Commons Attribution 4.0 International license:
   http://creativecommons.org/licenses/by/4.0/
*/

#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "arduino_pins.h"

// Defines
#define TWI_FAST_MODE

#ifdef TWI_FAST_MODE                 // TWI FAST mode timing limits. SCL = 100-400kHz
#define DELAY_T2TWI (_delay_us(2))   // >1.3us
#define DELAY_T4TWI (_delay_us(1))   // >0.6us
#else                                // TWI STANDARD mode timing limits. SCL <= 100kHz
#define DELAY_T2TWI (_delay_us(5))   // >4.7us
#define DELAY_T4TWI (_delay_us(4))   // >4.0us
#endif

#define TWI_NACK_BIT 0 // Bit position for (N)ACK bit

// Constants
// Prepare register value to: Clear flags, and set USI to shift 8 bits i.e. count 16 clock edges.
const unsigned char USISR_8bit = 1<<USISIF | 1<<USIOIF | 1<<USIPF | 1<<USIDC | 0x0<<USICNT0;
// Prepare register value to: Clear flags, and set USI to shift 1 bit i.e. count 2 clock edges.
const unsigned char USISR_1bit = 1<<USISIF | 1<<USIOIF | 1<<USIPF | 1<<USIDC | 0xE<<USICNT0;

void i2c_init(void);
uint8_t i2c_read(void);
uint8_t i2c_readLast(void);
bool i2c_write(uint8_t data);
bool i2c_start(uint8_t address, int readcount);
bool i2c_restart(uint8_t address, int readcount);
void i2c_stop(void);
uint8_t i2c_transfer(uint8_t data);

uint8_t i2c_read_register8(uint8_t deviceAddr, uint8_t deviceReg);
int16_t i2c_read_register16(uint8_t deviceAddr, uint8_t deviceReg);
void i2c_write_register8(uint8_t deviceAddr, uint8_t deviceReg, uint8_t value);
void i2c_write_register16(uint8_t deviceAddr, uint8_t deviceReg, uint8_t value);

#endif
