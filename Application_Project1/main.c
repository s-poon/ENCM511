/*
 * File:   main.c
 * Author: Steven, Anand, Jenna
 * Created on October 6, 2021, 11:43 AM
 */

// MPLAB header libraries
#include <xc.h>

#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
#include "IOs.h"
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "UART2.h"


//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config ICS = PGx2

//MAIN
void main(void) {
     
    // Change Clock
    NewClk(32); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
    InitUART2();
    IOinit(); // Initialize IO pins, pull up, and interrupts
    TimeDelayInit(); // Initialize the timer
    //InitUART2();
    

    
    //uint16_t result;
    while(1){
        IOcheck();
    }	

    
    
    return;
}

