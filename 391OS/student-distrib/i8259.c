/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 SOURCE: https://wiki.osdev.org/8259_PIC, Lecture 12 Slide 6
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
	outb(0xff, MASTER_8259_PORT_DATA);	/* mask all of 8259A-1 */
	outb(0xff, SLAVE_8259_PORT_DATA);	/* mask all of 8259A-2 */

	/*
	 * outb_p - this has to work on a wide range of PC hardware.
	 */
	outb(ICW1, MASTER_8259_PORT);	/* ICW1: select 8259A-1 init */
	outb(ICW2_MASTER, MASTER_8259_PORT_DATA);	/* ICW2: 8259A-1 IR0-7 mapped to 0x20-0x27 */
	outb(ICW3_MASTER, MASTER_8259_PORT_DATA);	/* 8259A-1 (the master) has a slave on IR2 */
	outb(ICW4, MASTER_8259_PORT_DATA);	/* master expects normal EOI */

	outb(ICW1, SLAVE_8259_PORT);	/* ICW1: select 8259A-2 init */
	outb(ICW2_SLAVE, SLAVE_8259_PORT_DATA);	/* ICW2: 8259A-2 IR0-7 mapped to 0x28-0x2f */
	outb(ICW3_SLAVE, SLAVE_8259_PORT_DATA);	/* 8259A-2 is a slave on master's IR2 */
	outb(ICW4, SLAVE_8259_PORT_DATA);	/* (slave's support for AEOI in flat mode
				    is to be investigated) */

	outb(0xff, MASTER_8259_PORT_DATA);	/* restore master IRQ mask */
	outb(0xff, SLAVE_8259_PORT_DATA);	/* restore slave IRQ mask */
    enable_irq(2);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {

    // Disable interrupts
    cli();

    uint16_t port;
    uint8_t value;
    
    // If irq 0-7, mask master, if irq 8-15, mask slave
    if(irq_num < 8) {
        port = MASTER_8259_PORT_DATA;
    } else {
        port = SLAVE_8259_PORT_DATA;
        irq_num -= 8;
    }

    // Logic for value
    value = inb(port) & ~(1 << irq_num);
    outb(value, port);

    // Enable interrupts
    sti();
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {

    // Disable interrupts
    cli();
    
    uint16_t port;
    uint8_t value;
 
    // If irq 0-7, mask master, if irq 8-15, mask slave
    if(irq_num < 8) {
        port = MASTER_8259_PORT_DATA;
    } else {
        port = SLAVE_8259_PORT_DATA;
        irq_num -= 8;
    }

    // Logic for value
    value = inb(port) | (1 << irq_num);
    outb(value, port);   

    // Enable interrupts
    sti(); 
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    // Disable interrupts
    cli();

    // Depending on whether or not irq num is on slave or master, use or logic with EOI signal to send EOI to correct port.
    if (irq_num >= 8)  {
        outb(EOI | (irq_num-8), SLAVE_8259_PORT);
        outb(EOI | 0x02, MASTER_8259_PORT);
    }
	outb(EOI | irq_num, MASTER_8259_PORT);

    // Enable interrupts
    sti();
}
