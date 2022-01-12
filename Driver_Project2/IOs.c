/*
 * File:   IOs.c
 * Author: Steven, Anand, Jenna
 *
 * Created on October 6, 2021, 11:41 AM
 */


#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"

uint8_t x;


void IOinit(){
    AD1PCFG = 0xFFFF;           // Turn all analog pins as digital
    
    TRISAbits.TRISA2 = 1;       // Sets RA2-GPIO7 as input
    TRISAbits.TRISA4 = 1;       // Sets RA4-GPIO10 as input     
    TRISBbits.TRISB4 = 1;       // Sets RA4-GPIO9 as input
    
    CNPU2bits.CN30PUE = 1;      // Sets CN30 as pull up (PU)
    CNPU1bits.CN0PUE = 1;       // Sets CN0 as pull up (PU)
    CNPU1bits.CN1PUE = 1;       // Sets CN1 as pull up (PU)
    
    TRISBbits.TRISB8 = 0;       // Sets RB0-GPIO4 as output
    
    
}

void IOcheck(){
    // Number of buttons being pressed at one time
    uint8_t buttonsPress = PORTAbits.RA2 + PORTAbits.RA4 + PORTBbits.RB4;
    
    // track LED on/off
    static uint8_t LED = 0;
    
    if(buttonsPress == 3){ // No PB are being pressed
        LED = 0; // Turn off LED
        LATBbits.LATB8 = LED; // Set output to 0 (turn off LED)
    }
    else if(buttonsPress <= 1){ // Two or more PB pressed together
        LED = 1; // Turn on LED
        LATBbits.LATB8 = LED; // Set output to 1 (turn on LED)
    }
    else if(PORTAbits.RA2 == 0){ // PB1 being pressed
        if(LED == 0){ // switch LED value to emulate blinking
            LED = 1; // Set LED to 1 if its 0
        }
        else{
            LED = 0; // Set LED to 0 if its 1
        } 
        LATBbits.LATB8 = LED; // Turn LED on/off    
        Delay_ms(1000); // 1 second delay using timer & interrupts
    }
    else if(PORTAbits.RA4 == 0){ // PB2 being pressed
        if(LED == 0){ // switch LED value to emulate blinking
            LED = 1; // Set LED to 1 if its 0
        }
        else{
            LED = 0; // Set LED to 0 if its 1
        } 
        LATBbits.LATB8 = LED; // Turn LED on/off    
        Delay_ms(2000); // 2 second delay using timer & interrupts
    }
    else if(PORTBbits.RB4 == 0){ // PB3 being pressed
        if(LED == 0){ // switch LED value to emulate blinking
            LED = 1; // Set LED to 1 if its 0
        }
        else{
            LED = 0; // Set LED to 0 if its 1
        } 
        LATBbits.LATB8 = LED; // Turn LED on/off    
        Delay_ms(3000); // 3 second delay using timer & interrupts
    }
}