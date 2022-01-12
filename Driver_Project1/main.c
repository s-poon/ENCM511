/* 
 * File:   main.c
 * Author: Steven Poon
 *
 * Created on 29 September 2021, 12:02
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#pragma config OSCIOFNC = ON                        // CLKO Enable Configuration bit (CLKO output disabled; pin functions as port I/O)
/*
 * 
 */

void main(void){
    TRISAbits.TRISA0 = 1;                           //Setting RA0 as input
    TRISAbits.TRISA1 = 1;                           //Setting RA1 as input
    TRISAbits.TRISA2 = 1;                           //Setting RA2 as input
    TRISAbits.TRISA3 = 1;                           //Setting RA3 as input          
    
    TRISBbits.TRISB0 = 0;                           //Setting RB0 as output
    TRISBbits.TRISB1 = 0;                           //Setting RB1 as output
    TRISBbits.TRISB2 = 0;                           //Setting RB2 as output
    TRISBbits.TRISB4 = 0;                           //Setting RB4 as output
    TRISBbits.TRISB7 = 0;                           //Setting RB7 as output
    TRISBbits.TRISB8 = 0;                           //Setting RB8 as output
    TRISBbits.TRISB9 = 0;                           //Setting RB9 as output
    TRISBbits.TRISB12 = 0;                          //Setting RB12 as output
    TRISBbits.TRISB13 = 0;                          //Setting RB13 as output
    
    uint16_t UCID[8] = {3, 0, 0, 9, 4, 4, 3, 3};    //Setting the UCID
    
    
    while(1){                                       //Infinite while loop
        if(PORTA == 0){
            LATB = 0;                               //If input is 0 no LEDs will be on
        }else if(PORTA > 0b1000){
            LATB = 0;                               //If input is greater than 10 no LEDs will be on
        }else{
            uint16_t input = UCID[PORTA - 1];       //Obtaining which digit of the UCID to get
            uint16_t output[9] = {};                //Creating empty array to store states
            uint8_t i;                              //for the for loop
            for(i = 0; i < 9; i ++){                //for loop to fill output with the appropriate HIGH or LOW state for each output
                if(i < input){                      //if i is less than the number in the UCID then the output will be high
                    output[i] = 1;                  //setting the output at i to high
                }
                else{                               //otherwise the output will be low
                    output[i] = 0;                  //setting the output at i to low

                }
            }
            
            //Turning on the corresponding LEDs based on the input
            LATBbits.LATB0 = output[0];             // setting RB0 to output[0]
            LATBbits.LATB1 = output[1];             // setting RB1 to output[1]
            LATBbits.LATB2 = output[2];             // setting RB2 to output[2]
            LATBbits.LATB4 = output[3];             // setting RB4 to output[3]
            LATBbits.LATB7 = output[4];             // setting RB7 to output[4]
            LATBbits.LATB8 = output[5];             // setting RB8 to output[5]
            LATBbits.LATB9 = output[6];             // setting RB9 to output[6]
            LATBbits.LATB12 = output[7];            // setting RB12 to output[7]
            LATBbits.LATB13 = output[8];            // setting RB13 to output[8]
        }
    }
    return;
}

