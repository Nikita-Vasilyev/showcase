#ifndef PIT_H_INCLUDED
#define PIT_H_INCLUDED


#include "lib.h"
#include "i8259.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND 0x43
#define PIT_FREQUENCY 1193182

#define DESIRED_FREQUENCY 100

void init_pit();

#endif
