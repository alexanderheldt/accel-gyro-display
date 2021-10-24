//Adapted from https://github.com/MarcelMG/AVR8_BitBang_UART_TX

#ifndef UART_H
#define UART_H

#define TX_PORT PORTB
#define TX_PIN  PB4
#define TX_DDR  DDRB
#define TX_DDR_PIN DDB4

void uart_init(void);
void uart_tx_str(char* character);
void uart_tx(char character);

#endif
