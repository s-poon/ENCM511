/*
 * File:   IOs.c
 * Author: Steven, Anand, Jenna
 *
 * Created on October 6, 2021, 11:41 AM
 */


#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "UART2.h"
#include "ChangeClk.h"

void IOinit(){
    AD1PCFG = 0xFFFF;           // Turn all analog pins as digital
    
    TRISAbits.TRISA2 = 1;       // Sets RA2-GPIO7 as input
    TRISAbits.TRISA4 = 1;       // Sets RA4-GPIO10 as input     
    TRISBbits.TRISB4 = 1;       // Sets RA4-GPIO9 as input
    TRISBbits.TRISB8 = 0;       // Sets RB0-GPIO4 as output
    TRISBbits.TRISB12 = 0;      // Sets RN12 as output
    TRISAbits.TRISA6 = 0;       // Sets RA6 as output
    TRISBbits.TRISB15 = 0;      // Sets RB1 as frequency port 
    
    
    
    CNPU2bits.CN30PUE = 1;      // Sets CN30 as pull up (PU)
    CNPU1bits.CN0PUE = 1;       // Sets CN0 as pull up (PU)
    CNPU1bits.CN1PUE = 1;       // Sets CN1 as pull up (PU)
    
    
    CNEN1bits.CN0IE = 1;        // Sets CN0 for CN Interrupt (input 1)
    CNEN1bits.CN1IE = 1;        // Sets CN1 for CN Interrupt (input 2)
    CNEN2bits.CN30IE = 1;       // Sets CN30 for CN Interrupt (input 3)
    IEC1bits.CNIE = 1;          // Enable interrupt by setting to 1
    IPC4bits.CNIP1 = 0b111;     // CN interrupt priority to highest (7)


}
