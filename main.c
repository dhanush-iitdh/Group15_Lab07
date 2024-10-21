#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

uint8_t receivedByte;
bool dataReceivedFlag = true;

void UART5_send(void);
void UART5_Transmit(uint8_t data);
void PortF_Init(void);
void PORTE_Init(void);
void UART5_Init(void);
uint8_t UART5_ReceiveByte(void);
void UART5_Read(void);

void PortF_Init(void) {
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   /* Enable clock for Port F */
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;      /* Unlock Port F */
    GPIO_PORTF_CR_R = 0x1f;                 /* Allow changes to PF0-PF4 */
    GPIO_PORTF_DEN_R = 0x1f;                /* Enable digital function for PF0-PF4 */
    GPIO_PORTF_DIR_R = 0x0E;                /* Set PF1-PF3 as outputs (LEDs) */
    GPIO_PORTF_PUR_R = 0x11;                /* Enable pull-up resistors for PF0 and PF4 (SW1, SW2) */
}

void PORTE_Init(void) {
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOE;  /* Enable GPIO Port E clock */
    SYSCTL_RCGCUART_R |= (1 << 5);            /* Enable UART5 clock */

    GPIO_PORTE_LOCK_R = GPIO_LOCK_KEY;        /* Unlock Port E */
    GPIO_PORTE_CR_R = 0xff;                   /* Allow changes to PE0-PE7 */
    GPIO_PORTE_DEN_R = 0x30;                  /* Enable digital for PE6 (RX) and PE7 (TX) */
    GPIO_PORTE_AFSEL_R = 0x30;                /* Enable alternate function for PE6 and PE7 */
    GPIO_PORTE_AMSEL_R = 0x00;                /* Disable analog function */
    GPIO_PORTE_PCTL_R &= ~0x00FF0000;         /* Clear PCTL for PE6 and PE7 */
    GPIO_PORTE_PCTL_R |= 0x00110000;          /* Set PCTL for UART */
}

void UART5_Init(void) {
    UART5_CTL_R = 0x00;                       /* Disable UART before configuration */
    UART5_IBRD_R = 104;                       /* Integer part of BRD = 16MHz / (16 * 9600) = 104 */
    UART5_FBRD_R = 11;                        /* Fractional part of BRD = 0.16 * 64 + 0.5 = 11 */
    UART5_LCRH_R = 0x72;                      /* 8 bits, odd parity, 1 stop bit */
    UART5_CTL_R = 0x301;                      /* Enable UART */
}

uint8_t UART5_ReceiveByte(void) {
    while (UART5_FR_R & 0x10) {}              /* Wait until RXFE is 0 */
    return UART5_DR_R;                        /* Read data */
}

void UART5_Read(void) {
    receivedByte = UART5_ReceiveByte();       /* Receive byte */

    if (dataReceivedFlag) {
        if (UART5_FR_R & 0x04) {              /* Check for RX FIFO not empty */
            GPIO_PORTF_DATA_R |= 0x08;        /* Turn on GREEN LED */
            GPIO_PORTF_DATA_R &= ~0x04;       /* Turn off BLUE LED */
            GPIO_PORTF_DATA_R &= ~0x02;       /* Turn off RED LED (Error) */
        } else {
            if (receivedByte == 0xAA) {
                GPIO_PORTF_DATA_R |= 0x08;    /* Turn on GREEN LED */
                GPIO_PORTF_DATA_R &= ~0x04;   /* Turn off BLUE LED */
                GPIO_PORTF_DATA_R &= ~0x02;   /* Turn off RED LED (Error) */
            } else if (receivedByte == 0xF0) {
                GPIO_PORTF_DATA_R &= ~0x08;   /* Turn off GREEN LED */
                GPIO_PORTF_DATA_R |= 0x04;    /* Turn on BLUE LED */
                GPIO_PORTF_DATA_R &= ~0x02;   /* Turn off RED LED (Error) */
            } else {
                GPIO_PORTF_DATA_R &= ~0x08;   /* Turn off GREEN LED */
                GPIO_PORTF_DATA_R &= ~0x04;   /* Turn off BLUE LED */
                GPIO_PORTF_DATA_R |= 0x02;    /* Turn on RED LED (Error) */
            }
        }
    }
}

void UART5_send(void) {
    if (!(GPIO_PORTF_DATA_R & 0x01)) {        /* Check if SW1 is pressed */
        UART5_Transmit(0xF0);                 /* Transmit 0xF0 */
        while (!(GPIO_PORTF_DATA_R & 0x01));  /* Wait until released */
    }
    if (!(GPIO_PORTF_DATA_R & 0x10)) {        /* Check if SW2 is pressed */
        UART5_Transmit(0xAA);                 /* Transmit 0xAA */
        while (!(GPIO_PORTF_DATA_R & 0x10));  /* Wait until released */
    }
}

void UART5_Transmit(uint8_t data) {
    while (UART5_FR_R & UART_FR_TXFF);        /* Wait until the transmit FIFO is not full */
    UART5_DR_R = data;                        /* Transmit data */
}

int main(void) {
    PortF_Init();                             /* Initialize Port F */
    PORTE_Init();                             /* Initialize Port E */
    UART5_Init();                             /* Initialize UART5 */

    while (1) {
        UART5_Read();                         /* Continuously read UART5 */
    }
}
