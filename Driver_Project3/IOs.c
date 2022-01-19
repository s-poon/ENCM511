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
    
    
    CNEN1bits.CN0IE = 1;
    CNEN2bits.CN30IE = 1;
    CNEN1bits.CN1IE = 1;
    IEC1bits.CNIE = 1;
    IPC4bits.CNIP1 = 1;
    IPC4bits.CNIP0 = 1;
    IPC4bits.CNIP2 = 1;
//    CNCONbits.ON = 1;
    
    
}

void IOcheck(){
    // Number of buttons being pressed at one time
    // track LED on/off
    
    static uint8_t LED = 0;
    //Disp2String("\n\rPB1 is pressed\n");
    if (x == 0){
        LATBbits.LATB8 = 1;
        Disp2String("\n\rPB2 and PB3 are pressed\n");
        Idle();
    }
    else if (x == 1){
        LATBbits.LATB8 = 1;
        Disp2String("\n\rPB1 and PB3 are pressed\n");
        Idle();
    }
    else if (x == 2){
        LATBbits.LATB8 = 1;
        Disp2String("\n\rPB1 and PB2 are pressed\n"); 
        Idle();
    }
    else if(x == 3){
        LATBbits.LATB8 = 1;   
        Disp2String("\n\rAll buttons are pressed\n");
        Idle();
    }
    else if(x == 4){ // PB1 being pressed
        if(LED == 0){ // switch LED value to emulate blinking
            LED = 1; // Set LED to 1 if its 0
        }
        else{
            LED = 0; // Set LED to 0 if its 1
        } 
        LATBbits.LATB8 = LED; // Turn LED on/off 
        Disp2String("\n\rPB1 is pressed\n");
        Delay_ms(100); // 0.5 second delay using timer & interrupts
    }
    else if(x == 5){ // PB2 being pressed
        if(LED == 0){ // switch LED value to emulate blinking
            LED = 1; // Set LED to 1 if its 0  
        }
        else{
            LED = 0; // Set LED to 0 if its 1
        } 
        LATBbits.LATB8 = LED; // Turn LED on/off 
        Disp2String("\n\rPB2 is pressed\n");
        Delay_ms(1200); // 2 second delay using timer & interrupts
    }
    else if(x == 6){ // PB3 being pressed
        if(LED == 0){ // switch LED value to emulate blinking
            LED = 1; // Set LED to 1 if its 0
        }
        else{
            LED = 0; // Set LED to 0 if its 1
        } 
        LATBbits.LATB8 = LED; // Turn LED on/off
        Disp2String("\n\rPB3 is pressed\n");
        Delay_ms(2200); // 3 second delay using timer & interrupts
    }
    else{
        LED = 0;
        LATBbits.LATB8 = LED;
        Disp2String("\n\rNo buttons are pressed\n"); 
        Idle();
        
    }
    //Idle();
}

void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void)
{
    if(IFS1bits.CNIF ==1){
        //Creating variables to check the state of each button when the interrupt is triggered
        uint16_t B1, B2, B3;
        B1 = PORTAbits.RA2;     // Mapping B1 to PIN A2
        B2 = PORTAbits.RA4;     // Mapping B2 to PIN A4
        B3 = PORTBbits.RB4;     // Mapping B3 to PIN B4
        x = 7;                  
        Delay_ms(10);
        if(B1 == PORTAbits.RA2 && B2 == PORTAbits.RA4 && B3 == PORTBbits.RB4){
            if(!B3 && !B2 && B1){
                x = 0;
            }
            else if(!B3 && !B1 && B2){
                x = 1;
            }
            else if(!B2 && !B1 && B3){
                x = 2;
            }
            else if(!B2 && !B1 && !B3){
                x = 3;
            }
            else if (!B1 && B2 && B3){
                x = 4;
            }
            else if (!B2 && B1 && B3){
                x = 5;
            }
            else if (!B3 && B1 && B2){
                x = 6;
            }
            else{
                x = 7;
                 
            }
        }
    }
    IFS1bits.CNIF = 0;
//    Nop();
}