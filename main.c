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

int main(void) {

    while (1) {

}

}
