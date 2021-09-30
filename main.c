/* 
 * File:   main.c
 * Author: Me
 *
 * Created on 29 September 2021, 12:02
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#pragma config OSCIOFNC = ON            // CLKO Enable Configuration bit (CLKO output disabled; pin functions as port I/O)
/*
 * 
 */

void main(void){
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;
    
    uint16_t UCID[8] = {3, 0, 0, 9, 4, 4, 3, 3};
    while(1){
        
        if(PORTA != 0){
            uint16_t input = UCID[PORTA - 1];
            uint16_t output[9] = {};
            uint8_t i;
            for(i = 0; i < 9; i ++){
                if(i < (input)){
                    //output[i] = 0;
                    output[i] = 1;
                }
                else{
                    output[i] = 0;

                }
            }
            LATBbits.LATB0 = output[0];
            LATBbits.LATB1 = output[1];
            LATBbits.LATB2 = output[2];
            LATBbits.LATB4 = output[3];
            LATBbits.LATB7 = output[4];
            LATBbits.LATB8 = output[5];
            LATBbits.LATB9 = output[6];
            LATBbits.LATB12 = output[7];
            LATBbits.LATB13 = output[8];
        }else{
            LATB = 0;
        }
    }
    return;
}

