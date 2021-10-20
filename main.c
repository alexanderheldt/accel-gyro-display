#include <avr/io.h>
#include <util/delay.h>

int main() {
  DDRB = (1 << DDB2);

  while(1) {
    _delay_ms(2000);
    PORTB ^= (1 << PB2);

    PORTB ^= (1 << PB2);
    _delay_ms(1000);
  }

  return(0);
}
