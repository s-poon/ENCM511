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

uint8_t x = 10;                 // x will track which button is being pressed
uint8_t minutes = 0;
uint8_t seconds = 10;
uint8_t start = 1;

void IOinit(){
     AD1PCFG = 0xFFFF;           // Turn all analog pins as digital
    
    TRISAbits.TRISA2 = 1;       // Sets RA2-GPIO7 as input
    TRISAbits.TRISA4 = 1;       // Sets RA4-GPIO10 as input     
    TRISBbits.TRISB4 = 1;       // Sets RB4-GPIO9 as input
    
    CNPU2bits.CN30PUE = 1;      // Sets CN30 as pull up (PU)
    CNPU1bits.CN0PUE = 1;       // Sets CN0 as pull up (PU)
    CNPU1bits.CN1PUE = 1;       // Sets CN1 as pull up (PU)
    
    TRISBbits.TRISB8 = 0;       // Sets RB0-GPIO4 as output
    
    
    CNEN1bits.CN0IE = 1;        // Sets CN0 for CN Interrupt (input 1)
    CNEN1bits.CN1IE = 1;        // Sets CN1 for CN Interrupt (input 2)
    CNEN2bits.CN30IE = 1;       // Sets CN30 for CN Interrupt (input 3)
    IEC1bits.CNIE = 1;          // Enable interrupt by setting to 1
    IPC4bits.CNIP1 = 1;         // CN interrupt priority to highest (7)
    IPC4bits.CNIP0 = 1;         // CN interrupt priority to highest (7)
    IPC4bits.CNIP2 = 1;         // CN interrupt priority to highest (7)
    
    
}

void IOcheck(){
    // Number of buttons being pressed at one time
    // track LED on/off
    
    static uint8_t LED = 0;                             // set led to off originally
    
    while (x == 4){                                     // PB1 is pressed
        minutes +=1;                                    // minutes are incremented by one
        if(minutes >= 60){                              // minutes have a max of 59
            minutes = 59;
            x = 10;
            Idle();
        }
        XmitUART2('\r', 1);                             // Start of the line
        XmitUART2(' ', 18);
        XmitUART2('\r', 1);                             // Start of the line
        Disp2Dec(minutes);                              // Display time
        Disp2String("m : ");
        Disp2Dec(seconds);
        Disp2String("s");
        Delay_ms(500);                                  // delays enough time to stop if needed
        if(PORTAbits.RA2 != 0){
            x = 10;
            if(start == 1){
                start = 0;
                }
                else {
                start = 1;
                }
            Idle();
        }
    }
    while(x == 5){                                      // PB2 is pressed
        seconds+=1;                                     // seconds are incremented by 1 
        if(seconds > 59){
            seconds = 59;
            x = 10;
            Idle();
        }
        XmitUART2('\r', 1);                             // Start of the line
        XmitUART2(' ', 18);
        XmitUART2('\r', 1);                             // Start of the line
        Disp2Dec(minutes);                              // Display Time
        Disp2String("m : ");
        Disp2Dec(seconds);
        Disp2String("s");
        Delay_ms(500);                                  // Delays long enough to stop if needed 
        if(PORTAbits.RA4 != 0 ){
            x = 10;
            if(start == 1){
                start = 0;
                }
                else {
                start = 1;
                }
            Idle();
        }
    }
    while(x ==7){                                       // PB 3 is pressed for longer than 3 seconds
        XmitUART2('\r', 1);                             // Start of the line
        XmitUART2(' ', 17);
        XmitUART2('\r', 1);                             // Start of the line
        Disp2String("00m : 00s");                       // Display time
        seconds = 0;
        minutes = 0; 
        start = 1;
        x = 10;
        Idle();
    }
    while(x == 8){                                      // PB3 pressed for less than 3 seconds
        if(start == 0){                                 // PB3 pressed and start 
            seconds-=1;                                 // decrement seconds 
            if(LED == 0){                               // switch LED value to emulate blinking
                LED = 1;                                // Set LED to 1 if its 0
            }
            else{
                LED = 0;                                // Set LED to 0 if its 1
            } 
            LATBbits.LATB8 = LED;
         
            if((seconds == 0) && (minutes == 0)){       //when seconds and minutes hits zero
                LATBbits.LATB8 = 1;                     //turns LED on
                Disp2String("\r00m : 00s -- ALARM");    //displays ALARM
                start = 1;
                x=10;
                Idle();
            }
            XmitUART2('\r', 1);                         // Start of the line
            XmitUART2(' ', 8);                          // Start of the line
            XmitUART2('\r', 1);                         // Start of the line
            Disp2Dec(minutes);                          // Displaying the time
            Disp2String("m : ");
            Disp2Dec(seconds);                  
            Disp2String("s");

            if((seconds == 0) && (minutes != 0)){       // checking if it is the end of a minute
                minutes -= 1;                           // decremeting the minute
                seconds = 60;                           // reseting seconds
            }
            LATBbits.LATB8 = LED;                       // turning LED on/off
            }
        if(start == 1){                                 //PB3 is pressed pause 
            x=10;
            Idle();
        }
    }
}


void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void)
{
    if(IFS1bits.CNIF ==1){ // if interrupt has occurred
        //Creating variables to check the state of each button when the interrupt is triggered
        uint16_t B1, B2, B3;                            // local variables to track buttons being pressed
        uint16_t seconds_on = 0;
        B1 = PORTAbits.RA2;                             // Mapping B1 to PIN A2
        B2 = PORTAbits.RA4;                             // Mapping B2 to PIN A4
        B3 = PORTBbits.RB4;                             // Mapping B3 to PIN B4       
        Delay_ms(10);                                   // used for for debouncing

        if(!B3 && B1 && B2){
        x = 6;
        }
        if (!B1 && B2 && B3 ){                          // if button 1 is being pressed
        x = 4;                                          // set to 4 to track
        }
        if (!B2 && B1 && B3){                           // if button 2 is being pressed
        x = 5;                                          // set to 5 to track
        }

        while(!B3 && B2 && B1 && x!= 7 && x!= 8){
            seconds_on +=1;                             //counts number of seconds that have passed
        XmitUART2('\r', 1);                             // Start of the line
        XmitUART2(' ', 8);                              // Start of the line
        XmitUART2('\r', 1);                             // Start of the line
        Disp2Dec(minutes);
        Disp2String("m : ");
        Disp2Dec(seconds);
        Disp2String("s");
        XmitUART2('\r', 1);     // Start of the line
            if(seconds_on >= 3 && PORTBbits.RB4 == 1){ //checks if button has been pressed for longer then 3 seconds
               x = 7;
            }
            else if (seconds_on < 3 && PORTBbits.RB4 == 1){ //checks if button has been pressed for less than 3 seconds
                x = 8;
                if(start == 1){
                start = 0;
                }
                else {
                start = 1;
                }
            }
        if(seconds == 0){
            seconds = 60;
        }
         }
    IFS1bits.CNIF = 0;                                  // reset interrupt flag
    }
}
