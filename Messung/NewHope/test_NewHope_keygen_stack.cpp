#include <stdint.h>
#include "NewHope.h"
#include "print.h"
#include "avr.h"
#include "RNG.h"

unsigned int ctr=0,newctr;
unsigned char canary=0xFF;
volatile unsigned char *p;
extern unsigned char _end; 
extern unsigned char __stack; 

static unsigned int stack_count(unsigned char canary)
{
  const unsigned char *p = &_end;
  unsigned int c = 0;
  while(*p == canary && p <= &__stack)
  {
    p++;
    c++;
  }
  return c;
} 

#define WRITE_CANARY(X) {p=X;while(p>= &_end) *(p--) = canary;}


main(void)
{ volatile unsigned char a; /* Mark the beginning of the stack */
  NewHopePrivateKey sk;
  uint8_t pk[1824];
  print("NewHope_KEM keygen (with RNG): \n");
  for (int i = 0; i < 15; i++)
  {
    WRITE_CANARY(&a);
    RNG.begin("Test");
    NewHope::keygen(pk,sk);
    newctr =(unsigned int)&a - (unsigned int)&_end - stack_count(canary);
    ctr = (newctr>ctr)?newctr:ctr;
  }
  print_stack("Keygen:",-1,newctr);
  serial_write(4);
  while(1) {};
  return 0;
}