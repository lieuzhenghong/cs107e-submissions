/*
 * LARSON SCANNER
 *
 * Currently this code is the blink code copied from lab 1.
 *
 * Modify the code to implement the larson scanner for assignment 1.
 *
 * Make sure to use GPIO pins 20-23 (or 20-27) for your scanner.
 */

.equ DELAY, 0x190000
.equ SMALLDELAY, 300
.equ LOWGPIO, 20
.equ HIGHGPIO, 23

// FSEL all GPIO pins from 20-23 to set them to output
// We do this by setting r1 to 000 000 000 000 001 001 001 001
setup:
    ldr r0, FSEL2
    mov r2, #HIGHGPIO
    sub r2, #LOWGPIO

    mov r1, #0
    setled:
        // multiplies by 3:
        // bitshift (multiplies by 2)
        // then plus r2
        mov r3, r2, lsl #1
        add r3, r2
        
        // The idea here is to do
        // mov r4, (1 << r3)
        // where r3 is 9, 6, 3, 0 etc
        // So r4 is now 1, 1000, 1000000 etc.
        // Then use orr to add r4 bytewise to r1
        mov r5, #1
        mov r4, r5, lsl r3
        orr r1, r1, r4

        // We decrement the loop and exit once r2 < 0
        subs r2, #1
        bge setled

    str r1, [r0]

    // R5 is direction register
    // if R5 is 1, we go forwards
    // if R5 is 0, we go backward
    mov r5, #0

    // R3 is the GPIO of the center LED
    mov r3, #LOWGPIO

// flipbit function flips R5 between 0 and 1
flipbit:
    eor r5, #1

loop:
    // delay
    mov r2, #DELAY

    one_blink:
        // gpio in r3 will be set to brightest
        bl brightness_max

        @ @ left gpio
        sub r3, #1
        bl brightness_low

        @ @ right gpio
        add r3, #2
        bl brightness_low

        sub r3, #1
        
        subs r2, #SMALLDELAY
        bge one_blink

    // As mentioned, if R5 is 1 we go forwards
    // otherwise we go backwards
    cmp r5, #1
    beq forward

    backward:
        sub r3, #1
        b next

    forward:
        add r3, #1
        b next

    next:
        // At next, we check if we've hit the ends
        // If we've hit either LOWGPIO or HIGHGPIO
        // we flip the direction
        cmp r3, #HIGHGPIO
        beq flipbit
        cmp r3, #LOWGPIO
        beq flipbit

    b loop


# Invariant
# The whole thing runs DELAY times
# Number of cycles on = Small Delay On 
# Number of cycles off = Small Delay Off
brightness_max:
    mov r6, #250
    b flick

brightness_mid:
    mov r6, #50
    b flick


brightness_low:
    mov r6, #5
    b flick

flick:
    mov r1, #1
    mov r1, r1, lsl r3
    ldr r0, SET0
    str r1, [r0]

    sub r2, r6
    small_delay:
        subs r6, #1
        bge small_delay

    // set CLR0 to hold the value in r3
    ldr r0, CLR0
    str r1, [r0]

    mov r8, #SMALLDELAY
    sub r7, r8, r6

    sub r2, r7
    small_delay2:
        subs r7, #1
        bge small_delay2
    
    bx lr



FSEL0: .word 0x20200000
FSEL1: .word 0x20200004
FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
SET1:  .word 0x20200020
CLR0:  .word 0x20200028
CLR1:  .word 0x2020002C

