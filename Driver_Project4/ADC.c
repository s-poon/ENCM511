/*
 * File:   ADC.c
 * Author: Steven, Anand, Jenna
 *
 * Created on November 23, 2021, 1:32 PM
 */

#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "UART2.h"
#include "ChangeClk.h"
#include "ADC.h"

uint8_t x = 10; // To track state

uint16_t do_ADC(void) {
   uint16_t ADCvalue;                                                   // Averaged out ADC Value
   if(x == 1 || x == 2 || x == 3){                                      // if any of the buttons are pressed initialize sampling and sample
    /* ------------- ADC INITIALIZATION ------------------*/
    // turns on ADC and initial configuration including ADC sampling and conversion trigger
    AD1CON1bits.ADON = 1;                                               // turn on ADC module
    // Configure ADC by setting bits in AD1CON1 register ? slides 13-14 ?..
    AD1CON1bits.FORM = 0b00;                                            // Data output format bits (set to Integer)
    AD1CON1bits.SSRC = 0b111;                                           // Conversion trigger source select bits for triggered ADC conversion
    AD1CON1bits.ASAM = 0;                                               // To initiate ADC sampling
  
    // selects Vref
    AD1CON2bits.VCFG = 0b000;                                           // Selects AVDD, AVSS (supply voltage to PIC) as Vref ?.. 
    AD1CON2bits.CSCNA = 0;                                              // Do not scan inputs
    AD1CON2bits.SMPI = 0b0000;                                          // Needed if interrupts are used (enables interrupt when conversion is complete)
    AD1CON2bits.BUFM = 0;                                               // Buffer configured to 16-word buffer
    AD1CON2bits.ALTS = 0;                                               // Always uses MUX a input multiplexer settings

    // Configure ADC by setting bits in AD1CON2 as shown in slides 15-16
    // Selects ADC inputs
    AD1CON3bits.ADRC = 0;                                               // Use system clock 
    AD1CON3bits.SAMC = 0b1111;                                          // Slowest sampling time = 31* 2 / fclk, I think this show to do 1000 measurements in 1 second?
    
    // AD1CON3bits.ADCS = 0b11111; // A/D conversion clock select bits (64*Tcy)
    // Configure the ADC?s sample time by setting bits in AD1CON3 shown in slide 17 ?.. 
    // Ensure sample time is 1/10th of signal being sampled ?.. 
    // Select and configure ADC input as shown in slides 18-20
    // change to "AD1CHSbits.CH0NA" if we are using MUX A (Im not sure what MUX we are using)
    AD1CHSbits.CH0NA = 0;                                               // Channel 0 negative input is Vr-
    AD1CHSbits.CH0SA = 0b0101;                                          // Channel 0 positive input for AN5

    // ADC Analog Input Setup
    AD1PCFG = 0x0000;                                                   // Pin configured in Analog mode, I/O port read disabled, A/D samples pin voltage 
    AD1CSSL = 0x0000;                                                   // Analog channel omitted from input scan
    /* ------------- ADC SAMPLING AND CONVERSION ------------------*/
    int counter = 1;
    // 16 bit register used to hold ADC converted digital output ADC1BUF0 (average of 1000 measurements)
    uint32_t ADCValueBig;                                               // 32 bit register to hold 1000 measurements
    int i = 0;                                                          // outer loop
    int j = 0;                                                          // inner loop
    for(i = 0; i < 10; i++){                                            // loop 10 times for 10 total measurements
        NewClk(8);                                                      // set clock to fast
        for(j = 0; j < 1000; j++){                                      // loop 1000 times for 1000 measurements
            AD1CON1bits.SAMP=1;                                         // start sampling
            while(!AD1CON1bits.DONE == 0){};                            // until buffer is filled
            ADCValueBig += ADC1BUF0;                                    // add the value measured
        }
        ADCValueBig = ADCValueBig / 1000;                               // average the value
        ADCvalue = ADCValueBig;                                         // assign it back to the 10-bit integer
        draw_graph(ADCvalue);                                           // draw the graph


        ADCValueBig = 0;                                                // reset the big value to repeat
        NewClk(32);                                                     // set clock to slow for the time delay
        Delay_ms(1000);                                                 // delay by 1 second to emulate the 1000 measurements every second
    }

    AD1CON1bits.SAMP=0;                                                 // Stop sampling
    AD1CON1bits.ADON=0;                                                 // Turn off ADC, ADC value stored in ADC1BUF0;
    x = 10;                                                             // reset the value so function does not repeat
   }
   
   return (ADCvalue);                                                   //returns 10 bit ADC output stored in ADC1BIF0 to calling function
}
void draw_graph(uint16_t ADCvalue){
    int graph_scale = ADCvalue / 33;                                    // output is now in range between 0-31
    XmitUART2('=', graph_scale);                                        // print bar chart
    XmitUART2(' ', 31 - graph_scale);                                   // clear rest of line
    XmitUART2('|', 1);                                                  // clear rest of line
    Disp2Hex(ADCvalue);                                                 // Display hex value at the end of bar
    XmitUART2('\r', 1);                                                 // return cursor
    // XmitUART2('\n', 1);                                              // uncomment if want each bar graph in new line

    return;
}


void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void)
{
    if(IFS1bits.CNIF ==1){                                              // if interrupt has occurred
        
        //Creating variables to check the state of each button when the interrupt is triggered
        uint16_t B1, B2, B3;                                            // local variables to track buttons being pressed
        B1 = PORTAbits.RA2;                                             // Mapping B1 to PIN A2
        B2 = PORTAbits.RA4;                                             // Mapping B2 to PIN A4
        B3 = PORTBbits.RB4;                                             // Mapping B3 to PIN B4       
        Delay_ms(10);                                                   // used for for debouncing

        if (!B1 && B2 && B3){                                           // if button 1 is being pressed
            x = 1;                                                      // set to 4 to track
            Idle();                                                     // no button can be pressed
        }
        else if (!B2 && B1 && B3){                                      // if button 2 is being pressed
            x = 2;                                                      // set to 5 to track
            Idle();                                                     // no button can be pressed
        }
        else if (!B3 && B1 && B2){                                      // if button 3 is being pressed
            x = 3;                                                      // set to 6 to track
            Idle();                                                     // no button can be pressed
        }
        else{                                                           // if no buttons are being pressed
            x = 10;                                                     // set to 10 to track
            Idle();                                                     // no button can be pressed
        }

    }
    
    IFS1bits.CNIF = 0;                                                  // reset interrupt flag
}
