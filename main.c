#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"


void UART_Init(void);
void GPIO_Init(void);
void UART_Transmit(uint8_t data);
uint8_t UART_Receive(void);
void GPIOF_Handler(void);       /* GPIO Port F interrupt handler */
void UART0_Handler(void);       /* UART0 interrupt handler */


void GPIO_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;          /* Enable clock for PORTF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* Unlock PORTF */
    GPIO_PORTF_CR_R = 0x1F;             /* Allow changes to PF0, PF1, PF2, PF3, PF4 */

    GPIO_PORTF_DIR_R = 0x0E;            /* Set PF1, PF2, PF3 as outputs (LEDs) */
    GPIO_PORTF_DEN_R = 0x1F;            /* Enable digital function for PF0, PF1, PF2, PF3, PF4 */
    GPIO_PORTF_PUR_R = 0x11;            /* Enable pull-up resistors for PF0, PF4 (SW1, SW2) */

    /* Configure GPIO interrupts for switches (PF0, PF4) */
    GPIO_PORTF_IS_R &= ~0x11;           /* Make PF0 and PF4 edge-sensitive */
    GPIO_PORTF_IBE_R &= ~0x11;          /* Interrupt is controlled by IEV */
    GPIO_PORTF_IEV_R &= ~0x11;          /* Falling edge trigger for PF0 and PF4 */
    GPIO_PORTF_ICR_R |= 0x11;           /* Clear prior interrupts */
    GPIO_PORTF_IM_R |= 0x11;            /* Unmask interrupts for PF0 and PF4 */

    NVIC_EN0_R |= 0x40000000;           /* Enable interrupt 30 for Port F in NVIC */
}

/* Function to initialize UART0 with baud rate 9600, 8-bit data, odd parity */
void UART_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;          /* Enable clock for UART0 */
    SYSCTL_RCGCGPIO_R |= 0x01;          /* Enable clock for PORTA */

    UART0_CTL_R &= ~0x01;               /* Disable UART0 during configuration */
    UART0_IBRD_R = 104;                 /* Set integer part of baud rate divisor (16 MHz / (16 * 9600)) */
    UART0_FBRD_R = 11;                  /* Set fractional part of baud rate divisor */
    
    UART0_LCRH_R = 0x72;                /* 8-bit, odd parity, 1 stop bit, FIFO enabled */
    UART0_CC_R = 0x0;                   /* Use system clock */
    UART0_CTL_R = 0x301;                /* Enable UART0, TXE, RXE */

    /* Configure PA0 and PA1 for UART */
    GPIO_PORTA_AFSEL_R |= 0x03;         /* Enable alternate functions for PA0 and PA1 */
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x11;  /* Configure PA0, PA1 for UART */
    GPIO_PORTA_DEN_R |= 0x03;           /* Enable digital function for PA0, PA1 */

    NVIC_EN0_R |= 0x20;                 /* Enable interrupt 5 for UART0 in NVIC */
}

int main(void) {
    UART_Init();                        /* Initialize UART */
    GPIO_Init();                        /* Initialize GPIO for switches and LEDs */

    while (1) {
        /* Check for SW1 press */
        if ((GPIO_PORTF_DATA_R & 0x10) == 0x00) {  /* SW1 is pressed */
            UART_Transmit(0xF0);  /* Transmit 0xF0 over UART */
        }
        /* Check for SW2 press */
        if ((GPIO_PORTF_DATA_R & 0x01) == 0x00) {  /* SW2 is pressed */
            UART_Transmit(0xAA);  /* Transmit 0xAA over UART */
        }

        /* Listen for incoming data */
        uint8_t received_data = UART_Receive();
        if (received_data == 0xAA) {
            GPIO_PORTF_DATA_R = 0x08;  /* Turn on GREEN LED */
        } else if (received_data == 0xF0) {
            GPIO_PORTF_DATA_R = 0x04;  /* Turn on BLUE LED */
        } else {
            GPIO_PORTF_DATA_R = 0x02;  /* Turn on RED LED (Error) */
        }
    }
}

/* Function to transmit data over UART */
void UART_Transmit(uint8_t data) {
    while ((UART0_FR_R & 0x20) != 0);   /* Wait until TXFF is clear */
    UART0_DR_R = data;                  /* Transmit the data */
}

/* Function to receive data over UART */
uint8_t UART_Receive(void) {
    while ((UART0_FR_R & 0x10) != 0);   /* Wait until RXFE is clear */
    return (uint8_t)(UART0_DR_R & 0xFF); /* Read and return the received data */
}

/* GPIO Port F interrupt handler */
void GPIOF_Handler(void) {
    if (GPIO_PORTF_RIS_R & 0x10) {       /* Check if SW1 caused the interrupt */
        GPIO_PORTF_ICR_R |= 0x10;        /* Clear interrupt flag for PF4 (SW1) */
        UART_Transmit(0xF0);             /* Transmit 0xF0 if SW1 is pressed */
    }

    if (GPIO_PORTF_RIS_R & 0x01) {       /* Check if SW2 caused the interrupt */
        GPIO_PORTF_ICR_R |= 0x01;        /* Clear interrupt flag for PF0 (SW2) */
        UART_Transmit(0xAA);             /* Transmit 0xAA if SW2 is pressed */
    }
}

/* UART0 interrupt handler */
void UART0_Handler(void) {
    if (UART0_MIS_R & 0x10) {            /* Check if RX interrupt is triggered */
        uint8_t received_data = UART_Receive();

        if (received_data == 0xAA) {
            GPIO_PORTF_DATA_R = 0x08;    /* Turn on GREEN LED */
        } else if (received_data == 0xF0) {
            GPIO_PORTF_DATA_R = 0x04;    /* Turn on BLUE LED */
        } else {
            GPIO_PORTF_DATA_R = 0x02;    /* Turn on RED LED (Error) */
        }

        UART0_ICR_R = 0x10;              /* Clear RX interrupt */
    }
}
