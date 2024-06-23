#include "keyboard.h"

/* Function to initialize PIC to handle keyboard interrupts */
void init_keyboard(){
    // Keyboard interrupts are IRQ1.
    enable_irq(1);
}
