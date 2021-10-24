#include "i2c.h"

uint8_t I2Ccount = 0;

void i2c_init(void) {
  PORT_USI |= 1<<PIN_USI_SDA;                 // Enable pullup on SDA
  PORT_USI_CL |= 1<<PIN_USI_SCL;              // Enable pullup on SCL

  DDR_USI_CL |= 1<<PIN_USI_SCL;               // Enable SCL as output
  DDR_USI |= 1<<PIN_USI_SDA;                  // Enable SDA as output

  USIDR = 0xFF;                               // Preload data register with "released level" data
  USICR = 0<<USISIE | 0<<USIOIE |             // Disable Interrupts
	  1<<USIWM1 | 0<<USIWM0 |             // Set USI in Two-wire mode
	  1<<USICS1 | 0<<USICS0 | 1<<USICLK | // Software stobe as counter clock source
	  0<<USITC;
  USISR = 1<<USISIF | 1<<USIOIF | 1<<USIPF | 1<<USIDC | // Clear flags,
	  0x0<<USICNT0;                       // and reset counter.
}

uint8_t i2c_read(void) {
  if ((I2Ccount != 0) && (I2Ccount != -1)) I2Ccount--;

  // Read a byte
  DDR_USI &= ~(1<<PIN_USI_SDA);               // Enable SDA as input
  uint8_t data = i2c_transfer(USISR_8bit);

  // Prepare to generate ACK (or NACK in case of End Of Transmission)
  if (I2Ccount == 0) USIDR = 0xFF; else USIDR = 0x00;
  i2c_transfer(USISR_1bit);                   // Generate ACK/NACK

  return data;                                // Read successfully completed
}

uint8_t i2c_readLast(void) {
  I2Ccount = 0;
  return i2c_read();
}

bool i2c_write(uint8_t data) {
  // Write a byte
  PORT_USI_CL &= ~(1<<PIN_USI_SCL);           // Pull SCL LOW
  USIDR = data;                               // Setup data
  i2c_transfer(USISR_8bit);                   // Send 8 bits on bus

  // Clock and verify (N)ACK from slave
  DDR_USI &= ~(1<<PIN_USI_SDA);               // Enable SDA as input
  if (i2c_transfer(USISR_1bit) & 1<<TWI_NACK_BIT) return false;

  return true;                                // Write successfully completed
}

// Start transmission by sending address
bool i2c_start(uint8_t address, int readcount) {
  if (readcount != 0) { I2Ccount = readcount; readcount = 1; }
  uint8_t addressRW = address<<1 | readcount;

  // Release SCL to ensure that (repeated) Start can be performed
  PORT_USI_CL |= 1<<PIN_USI_SCL;              // Release SCL
  while (!(PIN_USI_CL & 1<<PIN_USI_SCL));     // Verify that SCL becomes high
#ifdef TWI_FAST_MODE
  DELAY_T4TWI;
#else
  DELAY_T2TWI;
#endif

  // Generate Start Condition
  PORT_USI &= ~(1<<PIN_USI_SDA);              // Force SDA LOW
  DELAY_T4TWI;
  PORT_USI_CL &= ~(1<<PIN_USI_SCL);           // Pull SCL LOW
  PORT_USI |= 1<<PIN_USI_SDA;                 // Release SDA

  if (!(USISR & 1<<USISIF)) return false;

  // Write address
  PORT_USI_CL &= ~(1<<PIN_USI_SCL);           // Pull SCL LOW
  USIDR = addressRW;                          // Setup data
  i2c_transfer(USISR_8bit);        // Send 8 bits on bus

  // Clock and verify (N)ACK from slave
  DDR_USI &= ~(1<<PIN_USI_SDA);               // Enable SDA as input
  if (i2c_transfer(USISR_1bit) & 1<<TWI_NACK_BIT) return false; // No ACK

  return true;                                // Start successfully completed
}

bool i2c_restart(uint8_t address, int readcount) {
  return i2c_start(address, readcount);
}

void i2c_stop(void) {
  PORT_USI &= ~(1<<PIN_USI_SDA);              // Pull SDA low
  PORT_USI_CL |= 1<<PIN_USI_SCL;              // Release SCL
  while (!(PIN_USI_CL & 1<<PIN_USI_SCL));     // Wait for SCL to go high
  DELAY_T4TWI;
  PORT_USI |= 1<<PIN_USI_SDA;                 // Release SDA
  DELAY_T2TWI;
}

uint8_t i2c_transfer(uint8_t data) {
  USISR = data;                               // Set USISR according to data
					      // Prepare clocking.
  data = 0<<USISIE | 0<<USIOIE |              // Interrupts disabled
	 1<<USIWM1 | 0<<USIWM0 |              // Set USI in Two-wire mode
	 1<<USICS1 | 0<<USICS0 | 1<<USICLK |  // Software clock strobe as source
	 1<<USITC;                            // Toggle Clock Port
  do {
    DELAY_T2TWI;
    USICR = data;                             // Generate positive SCL edge
    while (!(PIN_USI_CL & 1<<PIN_USI_SCL));   // Wait for SCL to go high
    DELAY_T4TWI;
    USICR = data;                             // Generate negative SCL edge
  } while (!(USISR & 1<<USIOIF));             // Check for transfer complete

  DELAY_T2TWI;
  data = USIDR;                               // Read out data
  USIDR = 0xFF;                               // Release SDA
  DDR_USI |= (1<<PIN_USI_SDA);                // Enable SDA as output

  return data;                                // Return the data from the USIDR
}

uint8_t i2c_read_register8(uint8_t deviceAddr, uint8_t deviceReg) {
    i2c_start(deviceAddr, 0);
    i2c_write(deviceReg);
    i2c_restart(deviceAddr, 1);
    uint8_t value = i2c_read();
    i2c_stop();

    return value;
}

int16_t i2c_read_register16(uint8_t deviceAddr, uint8_t deviceReg) {
    i2c_start(deviceAddr, 0);
    i2c_write(deviceReg);
    i2c_restart(deviceAddr, 2);

    uint8_t msb = i2c_read();
    uint8_t lsb = i2c_read();

    i2c_stop();

    uint16_t value = msb << 8 | lsb;

    return value;
}

void i2c_write_register8(uint8_t deviceAddr, uint8_t deviceReg, uint8_t value) {
    i2c_start(deviceAddr, 0);
    i2c_write(deviceReg);
    i2c_write(value);
    i2c_stop();
}

void i2c_write_register16(uint8_t deviceAddr, uint8_t deviceReg, uint8_t value) {
    i2c_start(deviceAddr, 0);
    i2c_write(deviceReg);
    i2c_write((uint8_t)(value >> 8));
    i2c_write((uint8_t)value);
    i2c_stop();
}
