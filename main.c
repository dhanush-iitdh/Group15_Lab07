#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"


void GPIO_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;          /* Enable clock for PORTF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* Unlock PORTF */
    GPIO_PORTF_CR_R = 0x1F;             /* Allow changes to PF0, PF1, PF2, PF3, PF4 */

    GPIO_PORTF_DIR_R = 0x0E;            /* Set PF1, PF2, PF3 as outputs (LEDs) */
    GPIO_PORTF_DEN_R = 0x1F;            /* Enable digital function for PF0, PF1, PF2, PF3, PF4 */
    GPIO_PORTF_PUR_R = 0x11;            /* Enable pull-up resistors for PF0, PF4 (SW1, SW2) */
}
/* Function to initialize UART0 with baud rate 9600, 8-bit data, odd parity */
void UART_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;   /* Enable clock for UART0 */
    SYSCTL_RCGCGPIO_R |= 0x01;   /* Enable clock for PORTA */

    UART0_CTL_R &= ~0x01;        /* Disable UART0 during configuration */
    UART0_IBRD_R = 104;          /* Set integer part of baud rate divisor (16 MHz / (16 * 9600)) */
    UART0_FBRD_R = 11;           /* Set fractional part of baud rate divisor */
    
    UART0_LCRH_R = 0x72;         /* 8-bit, odd parity, 1 stop bit, FIFO enabled */
    UART0_CC_R = 0x0;            /* Use system clock */
    UART0_CTL_R = 0x301;         /* Enable UART0, TXE, RXE */

    /* Configure PA0 and PA1 for UART */
    GPIO_PORTA_AFSEL_R |= 0x03;  /* Enable alternate functions for PA0 and PA1 */
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x11; /* Configure PA0, PA1 for UART */
    GPIO_PORTA_DEN_R |= 0x03;    /* Enable digital function for PA0, PA1 */
}
int main(void) {

    while (1) {

}

}
