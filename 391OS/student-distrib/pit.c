#include "pit.h"

void init_pit() {
    uint16_t divisor = PIT_FREQUENCY / DESIRED_FREQUENCY;
    outb(0x36, PIT_COMMAND);
    outb((uint8_t)(divisor & 0xFF), PIT_CHANNEL0);
    outb((uint8_t)((divisor >> 8) & 0xFF), PIT_CHANNEL0);
    enable_irq(0);
}
