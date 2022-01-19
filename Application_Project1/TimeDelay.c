/*
 * File:   TimeDelay.c
 * Author: Steven, Anand, Jenna
 *
 * Created on October 6, 2021, 11:42 AM
 */


#include "xc.h"
#include "TimeDelay.h"
#include "IOs.h"

void TimeDelayInit(){
    T2CONbits.TSIDL = 0;                                // Continue module operation in Idle mode
    T2CONbits.T32 = 0;                                  // Timer 2 as 16-bit timer
    T2CONbits.TCS = 0;                                  // Internal clock for timer 2
}

void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void){
    // Vector Function TMR2 Timer 2 expired
    IFS0bits.T2IF = 0;                                  // Clear Timer 2 interrupt flag
    // or else re-trigger interrupt
    T2CONbits.TON = 0;                                  // Stop Timer2
    // TMR2flag = 1; // Global variable in case code needs to know if Timer2
                     // is triggered
}

void Delay_ms(uint16_t time_ms){
    
    // T2CON Configuration
    T2CONbits.TON = 1;                                  // Start Timer2
    
    // Timer 2 interrupt Configuration
    // setting highest priority
    IPC1bits.T2IP0 = 1;                                 // Sets interrupt priority (1-7)
    IPC1bits.T2IP1 = 1;                                 // Sets interrupt priority (1-7)
    IPC1bits.T2IP2 = 1;                                 // Sets interrupt priority (1-7)
    IEC0bits.T2IE  = 1;                                 // Enables interrupts by setting TxIE bit for Timer 2
    IFS0bits.T2IF  = 0;                                 // Clear Timer 2 interrupt flag 
    // Compute PR2
    PR2 = 16 * time_ms;                                 // PR2 * Prescaler * 2/fclk * 1000 = Desired time delay (in milliseconds)
    
    Idle();                                             // Stays here until interrupt
    
    
}
