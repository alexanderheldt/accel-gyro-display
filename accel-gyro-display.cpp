#include <avr/io.h>
#include <util/delay.h>
#include <SoftwareSerial.h>

SoftwareSerial s(2, 4); // RX, TX

int main() {
  DDRB = (1 << DDB4) | (1 << DDB2);
  s.begin(9600);

  while(1) {
    s.println("turning led on");
    PORTB ^= (1 << PB2);
    _delay_ms(1000);

    s.println("turning led off");
    PORTB ^= (1 << PB2);
    _delay_ms(1000);
  }

  return (0);
}
