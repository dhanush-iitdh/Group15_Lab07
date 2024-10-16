//*****************************************************************************
//
// Startup code for use with TI's Code Composer Studio.
//
// Copyright (c) 2011-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
//*****************************************************************************

#include <stdint.h>

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// External declaration for the reset handler that is to be called when the
// processor is started
//
//*****************************************************************************
extern void _c_int00(void);

//*****************************************************************************
//
// Linker variable that marks the top of the stack.
//
//*****************************************************************************
extern uint32_t __STACK_TOP;

//*****************************************************************************
//
// External declarations for the interrupt handlers used by the application.
//
//*****************************************************************************
// Add declarations for UART0 and GPIOF handlers based on your UART and GPIO setup
extern void UART0_Handler(void);     // Handler for UART0 interrupts
extern void GPIOF_Handler(void);     // Handler for GPIO Port F interrupts

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000 or at the start of
// the program if located at a start address other than 0.
//
//*****************************************************************************
#pragma DATA_SECTION(g_pfnVectors, ".intvecs")
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((uint32_t)&__STACK_TOP),  // The initial stack pointer
    ResetISR,                                  // The reset handler
    NmiSR,                                     // The NMI handler
    FaultISR,                                  // The hard fault handler
    IntDefaultHandler,                         // The MPU fault handler
    IntDefaultHandler,                         // The bus fault handler
    IntDefaultHandler,                         // The usage fault handler
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    IntDefaultHandler,                         // SVCall handler
    IntDefaultHandler,                         // Debug monitor handler
    0,                                         // Reserved
    IntDefaultHandler,                         // The PendSV handler
    IntDefaultHandler,                         // The SysTick handler
    GPIOF_Handler,                             // GPIO Port F interrupt handler
    IntDefaultHandler,                         // GPIO Port A
    IntDefaultHandler,                         // GPIO Port B
    IntDefaultHandler,                         // GPIO Port C
    IntDefaultHandler,                         // GPIO Port D
    IntDefaultHandler,                         // GPIO Port E
    UART0_Handler,                             // UART0 Rx and Tx interrupt handler
    IntDefaultHandler,                         // UART1 Rx and Tx
    IntDefaultHandler,                         // SSI0 Rx and Tx
    IntDefaultHandler,                         // I2C0 Master and Slave
    IntDefaultHandler,                         // PWM Fault
    IntDefaultHandler,                         // PWM Generator 0
    IntDefaultHandler,                         // PWM Generator 1
    IntDefaultHandler,                         // PWM Generator 2
    IntDefaultHandler,                         // Quadrature Encoder 0
    IntDefaultHandler,                         // ADC Sequence 0
    IntDefaultHandler,                         // ADC Sequence 1
    IntDefaultHandler,                         // ADC Sequence 2
    IntDefaultHandler,                         // ADC Sequence 3
    IntDefaultHandler,                         // Watchdog timer
    IntDefaultHandler,                         // Timer 0 subtimer A
    IntDefaultHandler,                         // Timer 0 subtimer B
    IntDefaultHandler,                         // Timer 1 subtimer A
    IntDefaultHandler,                         // Timer 1 subtimer B
    IntDefaultHandler,                         // Timer 2 subtimer A
    IntDefaultHandler,                         // Timer 2 subtimer B
    IntDefaultHandler,                         // Analog Comparator 0
    IntDefaultHandler,                         // Analog Comparator 1
    IntDefaultHandler,                         // Analog Comparator 2
    IntDefaultHandler,                         // System Control (PLL, OSC, BO)
    IntDefaultHandler,                         // FLASH Control
    GPIOF_Handler,                             // GPIO Port F interrupt handler
    IntDefaultHandler,                         // GPIO Port G
    IntDefaultHandler,                         // GPIO Port H
    UART0_Handler,                             // UART2 Rx and Tx interrupt handler
    IntDefaultHandler,                         // SSI1 Rx and Tx
    IntDefaultHandler,                         // Timer 3 subtimer A
    IntDefaultHandler,                         // Timer 3 subtimer B
    IntDefaultHandler,                         // I2C1 Master and Slave
    IntDefaultHandler,                         // Quadrature Encoder 1
    IntDefaultHandler,                         // CAN0
    IntDefaultHandler,                         // CAN1
    0,                                         // Reserved
    0,                                         // Reserved
    IntDefaultHandler,                         // Hibernate
    IntDefaultHandler,                         // USB0
    IntDefaultHandler,                         // PWM Generator 3
    IntDefaultHandler,                         // uDMA Software Transfer
    IntDefaultHandler,                         // uDMA Error
    IntDefaultHandler,                         // ADC1 Sequence 0
    IntDefaultHandler,                         // ADC1 Sequence 1
    IntDefaultHandler,                         // ADC1 Sequence 2
    IntDefaultHandler,                         // ADC1 Sequence 3
    0,                                         // Reserved
    0,                                         // Reserved
    IntDefaultHandler,                         // GPIO Port J
    IntDefaultHandler,                         // GPIO Port K
    IntDefaultHandler,                         // GPIO Port L
    IntDefaultHandler,                         // SSI2 Rx and Tx
    IntDefaultHandler,                         // SSI3 Rx and Tx
    IntDefaultHandler,                         // UART3 Rx and Tx
    IntDefaultHandler,                         // UART4 Rx and Tx
    IntDefaultHandler,                         // UART5 Rx and Tx
    IntDefaultHandler,                         // UART6 Rx and Tx
    IntDefaultHandler,                         // UART7 Rx and Tx
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    IntDefaultHandler,                         // I2C2 Master and Slave
    IntDefaultHandler,                         // I2C3 Master and Slave
    IntDefaultHandler,                         // Timer 4 subtimer A
    IntDefaultHandler,                         // Timer 4 subtimer B
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    0,                                         // Reserved
    IntDefaultHandler,                         // Timer 5 subtimer A
    IntDefaultHandler,                         // Timer 5 subtimer B
    IntDefaultHandler,                         // Wide Timer 0 subtimer A
    IntDefaultHandler,                         // Wide Timer 0 subtimer B
    IntDefaultHandler,                         // Wide Timer 1 subtimer A
    IntDefaultHandler,                         // Wide Timer 1 subtimer B
    IntDefaultHandler,                         // Wide Timer 2 subtimer A
    IntDefaultHandler,                         // Wide Timer 2 subtimer B
    IntDefaultHandler,                         // Wide Timer 3 subtimer A
    IntDefaultHandler,                         // Wide Timer 3 subtimer B
    IntDefaultHandler,                         // Wide Timer 4 subtimer A
    IntDefaultHandler,                         // Wide Timer 4 subtimer B
    IntDefaultHandler,                         // Wide Timer 5 subtimer A
    IntDefaultHandler,                         // Wide Timer 5 subtimer B
    IntDefaultHandler,                         // FPU
    0,                                         // Reserved
    0,                                         // Reserved
    IntDefaultHandler,                         // I2C4 Master and Slave
    IntDefaultHandler,                         // I2C5 Master and Slave
    IntDefaultHandler,                         // GPIO Port M
    IntDefaultHandler,                         // GPIO Port N
    IntDefaultHandler,                         // Quadrature Encoder 2
    0,                                         // Reserved
    0,                                         // Reserved
    IntDefaultHandler,                         // GPIO Port P (Summary or P0)
    IntDefaultHandler,                         // GPIO Port P1
    IntDefaultHandler,                         // GPIO Port P2
    IntDefaultHandler,                         // GPIO Port P3
    IntDefaultHandler,                         // GPIO Port P4
    IntDefaultHandler,                         // GPIO Port P5
    IntDefaultHandler,                         // GPIO Port P6
    IntDefaultHandler,                         // GPIO Port P7
    IntDefaultHandler,                         // GPIO Port Q (Summary or Q0)
    IntDefaultHandler,                         // GPIO Port Q1
    IntDefaultHandler,                         // GPIO Port Q2
    IntDefaultHandler,                         // GPIO Port Q3
    IntDefaultHandler,                         // GPIO Port Q4
    IntDefaultHandler,                         // GPIO Port Q5
    IntDefaultHandler,                         // GPIO Port Q6
    IntDefaultHandler,                         // GPIO Port Q7
    IntDefaultHandler,                         // GPIO Port R
    IntDefaultHandler,                         // GPIO Port S
    IntDefaultHandler,                         // PWM 1 Generator 0
    IntDefaultHandler,                         // PWM 1 Generator 1
    IntDefaultHandler,                         // PWM 1 Generator 2
    IntDefaultHandler,                         // PWM 1 Generator 3
    IntDefaultHandler                          // PWM 1 Fault
};

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.
//
//*****************************************************************************
void ResetISR(void)
{
    __asm("    .global _c_int00\n"
          "    b.w     _c_int00");
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.
//
//*****************************************************************************
static void NmiSR(void)
{
    while(1) { }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault interrupt.
//
//*****************************************************************************
static void FaultISR(void)
{
    while(1) { }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected interrupt.
//
//*****************************************************************************
static void IntDefaultHandler(void)
{
    while(1) { }
}
