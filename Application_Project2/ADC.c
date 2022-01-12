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
#include "CTMU.h"
#include <string.h>

uint8_t x = 10; // To track state
uint16_t R1 = 1000;
double vin = 3.25;
unsigned int t1;
unsigned int t2; 



uint16_t do_ADC(int pin) {
   uint16_t ADCvalue;           // ADC Value
    /* ------------- ADC INITIALIZATION ------------------*/
    // turns on ADC and initial configuration including ADC sampling and conversion trigger
    AD1CON1bits.ADON = 1;       // turn on ADC module ?.. // Configure ADC by setting bits in AD1CON1 register ? slides 13-14 ?..
    AD1CON1bits.FORM = 0b00;    // Data output format bits (set to Integer)
    AD1CON1bits.SSRC = 0b111;   // Conversion trigger source select bits for triggered ADC conversion
    AD1CON1bits.ASAM = 0;       // To initiate ADC sampling
  
    // selects Vref
    AD1CON2bits.VCFG = 0b000;   // Selects AVDD, AVSS (supply voltage to PIC) as Vref ?.. 
    AD1CON2bits.CSCNA = 0;      // Do not scan inputs
    AD1CON2bits.SMPI = 0b0000;  // Needed if interrupts are used (enables interrupt when conversion is complete)
    AD1CON2bits.BUFM = 0;       // Buffer configured to 16-word buffer
    AD1CON2bits.ALTS = 0;       // Always uses MUX a input multiplexer settings

    // Configure ADC by setting bits in AD1CON2 as shown in slides 15-16
    // Selects ADC inputs
    AD1CON3bits.ADRC = 0;       // Use system clock 
    AD1CON3bits.SAMC = 0b1111;  // Slowest sampling time = 31* 2 / fclk, I think this ishow to do 1000 measurements in 1 second?
    //Configure the ADC?s sample time by setting bits in AD1CON3 shown in slide 17 ?.. 
    // Ensure sample time is 1/10th of signal being sampled ?.. 
    // Select and configure ADC input as shown in slides 18-20
    // change to "AD1CHSbits.CH0NA" if we are using MUX A (Im not sure what MUX we are using)
    AD1CHSbits.CH0NA = 0;       // Channel 0 negative input is Vr-
    AD1CHSbits.CH0SA = pin;  // Channel 0 positive input for the selected pin

    // ADC Analog Input Setup
    AD1PCFG = 0x0000;           // Pin configured in Analog mode, I/O port read disabled, A/D samples pin voltage 
    AD1CSSL = 0x0000;           // Analog channel omitted from input scan
    
    /* ------------- ADC SAMPLING AND CONVERSION ------------------*/
    AD1CON1bits.SAMP = 1; // start sampling
    while(!AD1CON1bits.DONE == 0){}; // until buffer is filled
    ADCvalue = ADC1BUF0; // add the value measured

    Delay_ms(500); // delay by 0.5 of a second

    AD1CON1bits.SAMP=0; // Stop sampling
    AD1CON1bits.ADON=0; // Turn off ADC, ADC value stored in ADC1BUF0;
    return (ADCvalue); //returns 10 bit ADC output stored in ADC1BIF0 to calling function
}


void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void)
{
    if(IFS1bits.CNIF ==1){ // if interrupt has occurred
        //Creating variables to check the state of each button when the interrupt is triggered
        uint16_t B1, B2, B3; // local variables to track buttons being pressed
        B1 = PORTAbits.RA2;     // Mapping B1 to PIN A2
        B2 = PORTAbits.RA4;     // Mapping B2 to PIN A4
        B3 = PORTBbits.RB4;     // Mapping B3 to PIN B4       
        Delay_ms(10); // used for for debouncing
        
        if (!B1 && B2 && B3){ // if button 1 is being pressed
            x = 1; // set to 4 to track
        }
        else if (!B2 && B1 && B3){ // if button 2 is being pressed
            x = 2; // set to 5 to track
        }
        else if (!B3 && B1 && B2){ // if button 3 is being pressed
            x = 3; // set to 6 to track
            
        }
        else{ // if no buttons are being pressed
            x = 10;
        }
    }
    IFS1bits.CNIF = 0; // reset interrupt flag
}

void voltmeter(void){
    uint16_t  voltage;                          // A variable to store the Voltage from the adc
    float v;                                    // Variable for calculated voltage
    char volt [6];                              // String for printing
    if(x == 1){
        NewClk(8);                              // Setting new clock
        voltage = do_ADC(5);                    // Get ADC Voltage
        v = voltage*vin/1023;                   // Convert to Volts
        AD1PCFG = 0xFFFF;               
        sprintf(volt, "%1.3f", v);              // Convert to string
        
        Disp2String("VOLTMETER Voltage = ");    //Printing the voltage out
        Disp2String(volt);
        Disp2String(" V");
        XmitUART2(' ', 30);                     // Clear rest of line
        XmitUART2('\r', 1);                     // return cursor
        NewClk(32);                             // Slow clock back down
    }  
}

void ohmmeter(void){
    uint32_t ohm;                               // Variable to store Ohms
    double vout;                                // Variable to get ADC voltage
    if(x == 2){
        NewClk(8);                              // Speed up clock
        vout = (do_ADC(11)*vin/1024);           // Get ADC Voltage and convert to Volts
        AD1PCFG = 0xFFFF; 
        ohm = (vout*R1)/(vin-vout);             // Calculate Ohms
        
        
        Disp2String("OHMMETER Ohms = ");        // Print out Ohms
        Disp2Dec(ohm);
        Disp2String("ohms");
        XmitUART2(' ', 30);
        XmitUART2('\r', 1);                     // return cursor
        NewClk(32);                             // Speed Clock up
    }
}

void capacitance(void){
    if(x == 3){
        char capa[5];
        float cap = do_CTMU();
        sprintf(capa, "%1.3f", cap);            // Convert to string
        
        Disp2String("Capacitance = ");          //Printing the capacitance out
        Disp2String(capa);
        Disp2String(" F");
        XmitUART2(' ', 30);                     // Clear rest of line
        XmitUART2('\r', 1);                     // return cursor
        NewClk(32);                             // Slow clock back down
    }
}


