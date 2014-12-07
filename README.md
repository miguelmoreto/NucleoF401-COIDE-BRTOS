NucleoF401-COIDE-BRTOS
======================

Example COIDE project for ST Nucleo F401RE (STM32F401RE) with BRTOS.

Tested with ARM GCC and Coocox COIDE 1.7.7

BRTOS stands for Brazilian Real-Time Operating System. It is a lightweight 
preemptive real time operating system designed for low end microcontrollers.

More info about BRTOS:

https://code.google.com/p/brtos/
https://brtosblog.wordpress.com/

* About this demo *

Clock is configured as:
 * External 8MHz from STLINK V2.
 * Main clock: 84MHz
 * AHB clock: 84MHz
 * APB1 clock: 42MHz
 * APB2 clcok: 84MHz
 * Timers clock: 84MHz
 * SDIO clock: 48MHz

Flash Prefetch enabled and 2 wait states (minimum for 84MHz and 3.3V).

This demo uses two tasks. One of them flashes the LED and prints a
string with a counter in serial every second.

The second task is used to handle the user button switch press event. When the user press the
button a string is printed in serial.

Using a mutex to avoid conflicts with two tasks sharing the serial.

Serial used is USART2 that is connected in STLINK V2-1 and becomes 
a Virtual Serial Com port when STLINK is connected.

Moreto

Florianopolis - Brazil - 2014
