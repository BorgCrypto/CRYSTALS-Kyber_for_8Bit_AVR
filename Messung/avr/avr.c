/* Taken from Curve25519 for AVR ATmega by Peter Schwabe */
/* URL: http://munacl.cryptojedi.org/curve25519-atmega.shtml*/
#include "avr.h"
#include "print.h"

void avr_end()
{
  serial_write(4);
  while(1) {};
}
