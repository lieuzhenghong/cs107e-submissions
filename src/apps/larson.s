/*
 * LARSON SCANNER
 *
 * Currently this code is the blink code copied from lab 1.
 *
 * Modify the code to implement the larson scanner for assignment 1.
 *
 * Make sure to use GPIO pins 20-23 (or 20-27) for your scanner.
 */

.equ DELAY, 0x1F0000

// FSEL all GPIO pins from 20-23 to set them to output
// We do this by setting r1 to 000 000 000 000 001 001 001 001
ldr r0, FSEL2
// Because mov can't handle too large constants, 
// we have to add byte by byte (hence the 512)
mov r1, #73
orr r1, r1, #512
str r1, [r0]

setr2:
    mov r3, #20

loop: 
    // We set the r3-th bit of SET0
    mov r1, #1
    mov r1, r1, lsl r3

    // set SET0 to hold the value in r3
    ldr r0, SET0
    str r1, [r0] 

    // delay
    mov r2, #DELAY
    wait1:
        subs r2, #1
        bne wait1

    // set SET0 to hold the value in r3
    ldr r0, CLR0
    str r1, [r0] 

    // delay
    mov r2, #DELAY
    wait2:
        subs r2, #1
        bne wait2

    // We subtract 23 from r3
    // and if it eq 0 (we have reached pin 23)
    // we start again from pin 20 (see setr2)
    subs r4, r3, #23
    beq setr2

    add r3, #1
    b loop

FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C

