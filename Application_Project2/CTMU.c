
#include <p24F16KA101.h>
#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include "UART2.h"
#include "ChangeClk.h"
#include "ADC.h"

#include "xc.h"
#include "CTMU.h"
#include "TimeDelay.h"
#include "UART2.h"


void CTMUInit(){
    CTMUCONbits.CTMUEN = 0; //Disable, enable in do_CTMU
    CTMUCONbits.CTMUSIDL = 0; //Continue operation in idle mode
    CTMUCONbits.TGEN = 0; //Disable edge delay
    CTMUCONbits.EDGEN = 0; //Disable edges
    CTMUCONbits.EDGSEQEN = 0; //No edge sequence needed
    CTMUCONbits.IDISSEN = 0; //Do not discharge to ground.
    CTMUCONbits.CTTRIG = 0; //Trigger output disabled. Enable current sources in SW
    
    CTMUICONbits.ITRIM = 0b111010; //Nominal current output
    CTMUICONbits.IRNG = 0b10; //10X base current 
    
    CTMUCONbits.EDG1STAT = 0;
    CTMUCONbits.EDG2STAT = 1;
    
    ADC1_for_CTMU_init();
}

void ADC1_for_CTMU_init(void){
    
    AD1CON1bits.ADON = 1; //Turn on ADC module
    
    //Read on AD11 (and only on AD5)
    AD1CSSLbits.CSSL0 = 0;
    AD1CSSLbits.CSSL1 = 0;
    AD1CSSLbits.CSSL2 = 0;
    AD1CSSLbits.CSSL3 = 0;
    AD1CSSLbits.CSSL4 = 0;
    AD1CSSLbits.CSSL5 = 0;
    AD1CSSLbits.CSSL10 = 0;
    AD1CSSLbits.CSSL11 = 0; //Omit from input scan
    AD1CSSLbits.CSSL12 = 0;
    
    AD1PCFGbits.PCFG12 = 0; //Analog read (set as analog pin)
    
    AD1CHSbits.CH0NB = 0;
    AD1CHSbits.CH0NA = 0; //Negative input is Vref-

    AD1CHSbits.CH0SB = 0b1011;
    AD1CHSbits.CH0SA = 12; //Ch0 input is AN11/RB13
    
    AD1CON1bits.ADSIDL = 0; //Continue in idle
    AD1CON1bits.FORM = 0b00;
    AD1CON1bits.SSRC = 0b000; //Clearing SAMP bit starts conversion
    AD1CON1bits.ASAM = 0; //Start sampling when SAMP bit is set

    AD1CON2bits.VCFG = 0b000; //Vr+ = AVDD, Vr- = AVSS.
    AD1CON2bits.OFFCAL = 0;
    AD1CON2bits.CSCNA = 0; // Don't scan inputs
    AD1CON2bits.SMPI = 0b0000;
    AD1CON2bits.BUFM = 0; //Buffer configured as 16-bit word
    AD1CON2bits.ALTS = 0; //Always use MUXA as MUX

    AD1CON3bits.ADRC = 1; //So that it can run in sleep mode
    AD1CON3bits.SAMC = 0b11111;
    AD1CON3bits.ADCS = 0b11111;
    
    TRISBbits.TRISB12 = 1;

    //Interrupts
    IPC3bits.AD1IP =  0x5; //Priority       
    IFS0bits.AD1IF = 0; // clear interrupt flag
    //IEC0bits.AD1IE = 1; //enable
    IEC0bits.AD1IE = 0; //disable
        
}

float do_CTMU(void){
    CTMUCONbits.CTMUEN = 1; //Disable, enable in do_CTMU
    
    AD1CON1bits.SAMP = 1;
    CTMUCONbits.EDG1STAT = 0;    
    CTMUCONbits.IDISSEN = 1;
    Delay_ms(100);

    CTMUCONbits.IDISSEN = 0;    
    CTMUCONbits.EDG2STAT = 0;
    CTMUCONbits.EDG1STAT = 1;
    Delay_ms(70);
    
    AD1CON1bits.SAMP = 0;
    while(AD1CON1bits.DONE == 0){}
    
    CTMUCONbits.EDG1STAT = 0;
    unsigned int raw = ADC1BUF0;
    float voltage = raw * 0.003173; //The coefficient is 3.25/1024
    
    return 0.0055*0.07/voltage;

}





































///*
//// TAKEN FROM D2L
//void CTMUInit(void){
//    
//    //CTMUCON - CTMU Control register
//     
//    CTMUCON = 0x0000; //make sure CTMU is disabled
//    CTMUCONbits.CTMUEN = 0;
//    CTMUCONbits.TGEN = 1;
//    CTMUCONbits.EDG1POL = 1;
//    CTMUCONbits.EDG2POL = 1;
//    
//    CTMUCONbits.EDG1SEL = 0b10;
//    CTMUCONbits.EDGSEQEN = 1;
//    CTMUCONbits.EDGEN = 1;
//    CTMUCONbits.EDG1STAT = 0;
//   
//    // CTMU continues to run when emulator is stopped,CTMU continues
//    // to run in Idle mode, Time Generation mode enabled, Edges are
//    // blocked. No edge sequence order, Analog current source not
//    // grounded, trigger output disabled, Edge2 polarity = positive level,
//    // Edge2 source = source 0, Edge1 polarity = positive level,
//    // Edge1 source = source 0, Set Edge status bits to zero
//    //CTMUICON - CTMU Current Control Register
//    CTMUICON = 0x0000; 
//    CTMUICONbits.IRNG0 = 1; //.55uA, Nominal - No Adjustment
//    CTMUICONbits.IRNG1 = 0; //.55uA, Nominal - No Adjustment
//    /**************************************************************************/
//    /* Set up AD Converter ****************************************************/
//    /**************************************************************************/
//    
//    
//    AD1CON1bits.ADON=0;            //Turn off ADC, turn on before sampling in do_ADC()
//    AD1CON1bits.ADSIDL=0;        //Continue ADC operation in idle mode
//    AD1CON1bits.FORM=0b00;        //0b00: Int, 01: Signed Int, 10:Fractional, 11:Signed Fract
//    AD1CON1bits.SSRC=0b000;    //Start ADC conversion Time set by SAMC has passed
//    AD1CON1bits.ASAM=0;     //Next Sampling begins when SAMP bit is set
//    AD1CON1bits.SAMP=0;        //Set this bit in ADC routine to start sampling  
//
//    // Sampling time and ADC Clk select
//    AD1CON2bits.VCFG=0b000;     //Vref+=AVDD   Vref-=AVSS
//    AD1CON2bits.CSCNA=0;    //Do not scan inputs; use channel selected by CH0SA bits
//    AD1CON2bits.SMPI=0b0000; //Any ADC interrrups occur at completion of each sample/convert seq.
//    AD1CON2bits.BUFM=0; //Buffer confugured as 1 16 word buffer 
//    AD1CON2bits.ALTS=0; //Always use MUXA input MUX settings
//    AD1CON3bits.ADRC=0; //0: Use System clk; 1: ADC uses internal RC clock for idle or sleep mode
//    AD1CON3bits.SAMC=0; 
//    AD1CON3bits.ADCS=0; //ADCS conversion clock select bits. Ignored if using internal AD RC clock. 
//    
//    // MUX settings
//    AD1CHSbits.CH0NA = 0;    //Ch0 -ve Input is VR-
//    AD1CHSbits.CH0SA = 12;    //YOUR ADC CHANNEL HERE
//   
//    //IO port selection
//    TRISBbits.TRISB12 = 1;  //SET YOUR PIN AS AN INPUT
//    AD1PCFGbits.PCFG12 = 0;     //SET YOUR PIN AS AN ANALOG INPUT
//    AD1CON1bits.ADON=1;
//    
//}
//
//// TAKEN FROM D2L
//uint16_t CTMU(void){
//    
//    //Clearing TMR2 and resetting flag
//    TMR2 = 0;
//    IFS0bits.T2IF = 0;
//    
//    T2CONbits.TSIDL = 0; // make program continue when idle
//    T2CONbits.T32 = 0; // do not use 32 bit timer use 16 bit instead
//    T2CONbits.TCS = 0; // use internal clk
//    
//    T2CONbits.TCKPS0 = 1;
//    T2CONbits.TCKPS1 = 1;
//    
//    T2CONbits.TON = 1; // turn on the CLK
//    
//    int i;
//    int j = 0; //index for loop
//    uint16_t Vread = 0;
//    uint16_t Vavg = 0;
//    double VTot = 0;
//    //assume CTMU and A/D have been setup correctly
////    CTMUInit();
//    CTMUCONbits.CTMUEN = 1;//Enable the CTMU
//    
//    CTMUICONbits.IRNG = 0b01;
//    
//    CTMUICONbits.ITRIM = 0;
//    
////    for(i = 0; i < 1; i++)
////    {
//        AD1CON1bits.SAMP = 1;  //Manual sampling start - move to after IDISSEN=0 if long time delay
//        CTMUCONbits.IDISSEN = 1; //drain charge on the circuit
//       
//
//        //[   TIME DELAY HERE   ]
//        // time delay for about 14 microseconds
//        int a = 0;
//        int time = 10000;
////        for(a = 0; a < 30000; a++) Delay_ms(1);
////        for(a = 0; a < 30000; a++) Delay_ms(1);
////        for(a = 0; a < 10000; a++) Delay_ms(1);
////        Disp2String("TEST 1 SECOND CAP");
//        Delay_ms(1000);
//        CTMUCONbits.IDISSEN = 0; //end drain of circuit
//        
//        CTMUCONbits.EDG1STAT = 1; //Begin charging the circuit
//        //using CTMU current source
//        Delay_ms(10);      // delay by about 71 us 
//
//        //[     TIME DELAY HERE    ]
////        for(a = 0; a < 30000; a++) Delay_ms(1);
////        for(a = 0; a < 30000; a++) Delay_ms(1);
////        for(a = 0; a < 10000; a++) Delay_ms(1);
////        Disp2String("TEST 2 SECOND CAP");
//        
//        
//        CTMUCONbits.EDG1STAT = 0; //Stop charging circuit
//        IFS0bits.AD1IF = 0; //make sure A/D Int not set
//        AD1CON1bits.SAMP = 0; //and begin A/D conv.
//        while(!AD1CON1bits.DONE); //Wait for A/D convert complete
//        Vread = ADC1BUF0; //Get the value from the A/D
//        IFS0bits.AD1IF = 0; //Clear A/D Interrupt Flag
//        VTot += Vread; //Add the reading to the total
//       
////    }
//    //If you are using multiple loops, don't forget to divide to average
//    Vavg = ((double)VTot/1); //1 Measurement
//    
////    char daString[60];
////        
////        sprintf(daString, "%.2f", VTot);
////        Disp2String(daString);
////    Disp2Dec(Vavg);
////    XmitUART2('\n', 1);
////    XmitUART2('\r', 1);
//    VTot = 0;
//    
//    CTMUCONbits.CTMUEN = 0;
//    
//    T2CONbits.TON = 0;
//    
//    
//    
//    return Vread;
//    
//    
//}







//void CTMU_Init(void){
//    CTMUICONbits.IRNG = 0b01;       //using the base current of 0.55uA
//    CTMUICONbits.ITRIM = 0;         //nominal current
//    CTMUCONbits.EDG1SEL = 0b00;     //using timer 1
//    CTMUCONbits.EDG2SEL = 0b11      //selecting CTED1 aka pin 14
//    CTMUCONbits.EDG1POL = 0;        //negative edge response (high to low)
//    CTMUCONbits.EDG2POL = 0;        //negative edge response (high to low)
//    CTMUCONbits.EDGSEQEN = 1;       //enables edge sequencing
//    CTMUCONbits.CTTRIG = 1;         //triggers ad conversion
//    CTMUCONbits.IDISSEN = 1;        //Discharging Circuit
//    Delay_ms(100);                  //waiting for circuit to discharge
//    CTMUCONbits.IDISSEN = 1;        //clearing idissen bit
//    CTMUCONbits.EDG1STAT = 0;       //Clearing edge status bit
//    CTMUCONbits.EDG2STAT = 0;       //Clearing edge status bit
//    CTMUCONbits.EDGEN = 1;          //enabling edge inputs
//    CTMUCONbits.CTMUEN = 1;
//   
//}



//
//
//int i;
//int j = 0; //index for loop
//unsigned int Vread = 0;
//unsigned int Vavg = 0;
//unsigned int VTot = 0;
////assume CTMU and A/D have been setup correctly
////see Example 11-1 for CTMU & A/D setup
//void CTMU(void){
//    CTMUCONbits.CTMUEN = 1;//Enable the CTMU
//    AD1CON1bits.ADON=0;            //Turn off ADC, turn on before sampling in do_ADC()
//    AD1CON1bits.ADSIDL=0;        //Continue ADC operation in idle mode
//    AD1CON1bits.FORM=0b00;        //0b00: Int, 01: Signed Int, 10:Fractional, 11:Signed Fract
//    AD1CON1bits.SSRC=0b000;    //Start ADC conversion Time set by SAMC has passed
//    AD1CON1bits.ASAM=0;     //Next Sampling begins when SAMP bit is set
//    AD1CON1bits.SAMP=0;        //Set this bit in ADC routine to start sampling  
//
//    // Sampling time and ADC Clk select
//    AD1CON2bits.VCFG=0b000;     //Vref+=AVDD   Vref-=AVSS
//    AD1CON2bits.CSCNA=0;    //Do not scan inputs; use channel selected by CH0SA bits
//    AD1CON2bits.SMPI=0b0000; //Any ADC interrrups occur at completion of each sample/convert seq.
//    AD1CON2bits.BUFM=0; //Buffer confugured as 1 16 word buffer 
//    AD1CON2bits.ALTS=0; //Always use MUXA input MUX settings
//    AD1CON3bits.ADRC=0; //0: Use System clk; 1: ADC uses internal RC clock for idle or sleep mode
//    AD1CON3bits.SAMC=0; 
//    AD1CON3bits.ADCS=0; //ADCS conversion clock select bits. Ignored if using internal AD RC clock. 
//
//    // MUX settings
////    AD1CHSbits.CH0NB=0;    //Ch0 MUXB -ve Input is VR-
////    AD1CHSbits.CH0SB=0bXXXX;    //YOUR ADC CHANNEL HERE
//    AD1CHSbits.CH0NA = 0;    //Ch0 -ve Input is VR-
//    AD1CHSbits.CH0SA = 12;    //YOUR ADC CHANNEL HERE
//
//    //IO port selection
//    TRISBbits.TRISB12 = 1 ; //SET YOUR PIN AS AN INPUT
//    AD1PCFGbits.PCFG12 = 0;     //SET YOUR PIN AS AN ANALOG INPUT
//    AD1CON1bits.ADON=1;
//
//    
//    for(j = 0; j < 2; j ++){
//        AD1CON1bits.SAMP = 1;  //Manual sampling start - move to after IDISSEN=0 if long time delay
//        CTMUCONbits.IDISSEN = 1; //drain charge on the circuit
//
//
//        Delay_ms(1000);
//
//        CTMUCONbits.IDISSEN = 0; //end drain of circuit
//
//        CTMUCONbits.EDG1STAT = 1; //Begin charging the circuit
////using CTMU current source
//
//
//        Delay_ms(100);
//
//        CTMUCONbits.EDG1STAT = 0; //Stop charging circuit
//        IFS0bits.AD1IF = 0; //make sure A/D Int not set
//        AD1CON1bits.SAMP = 0; //and begin A/D conv.
//        while(!AD1CON1bits.DONE); //Wait for A/D convert complete
//        Vread = ADC1BUF0; //Get the value from the A/D
//        IFS0bits.AD1IF = 0; //Clear A/D Interrupt Flag
//        VTot += Vread; //Add the reading to the total
//
//    }
////If you are using multiple loops, don't forget to divide to average
//    Vavg = (VTot)/2; //1 Measurement
//
//    Disp2Dec(Vavg);
//    XmitUART2('\n', 1);
//    XmitUART2('\r', 1);
//    VTot = 0;
//    
//
//}

//And this is the initialization code:

//void CTMUInit(void){ //CTMUCON - CTMU Control register
//
//    CTMUCON = 0x0000; //make sure CTMU is disabled
//    CTMUCONbits.CTMUEN = 0;
//    CTMUCONbits.TGEN = 1;
//    CTMUCONbits.EDG1POL = 1;
//    CTMUCONbits.EDG2POL = 1;
//
//    // CTMU continues to run when emulator is stopped,CTMU continues
//    // to run in Idle mode, Time Generation mode enabled, Edges are
//    // blocked. No edge sequence order, Analog current source not
//    // grounded, trigger output disabled, Edge2 polarity = positive level,
//    // Edge2 source = source 0, Edge1 polarity = positive level,
//    // Edge1 source = source 0, Set Edge status bits to zero
//    //CTMUICON - CTMU Current Control Register
//    CTMUICON = 0x0000; 
//    CTMUICONbits.IRNG0 = 1; //.55uA, Nominal - No Adjustment
//    CTMUICONbits.IRNG1 = 0; //.55uA, Nominal - No Adjustment
//    /**************************************************************************/
//    /* Set up AD Converter ****************************************************/
//    /**************************************************************************/
//
////
////    AD1CON1bits.ADON=0;            //Turn off ADC, turn on before sampling in do_ADC()
////    AD1CON1bits.ADSIDL=0;        //Continue ADC operation in idle mode
////    AD1CON1bits.FORM=0b00;        //0b00: Int, 01: Signed Int, 10:Fractional, 11:Signed Fract
////    AD1CON1bits.SSRC=0b000;    //Start ADC conversion Time set by SAMC has passed
////    AD1CON1bits.ASAM=0;     //Next Sampling begins when SAMP bit is set
////    AD1CON1bits.SAMP=0;        //Set this bit in ADC routine to start sampling  
////
////    // Sampling time and ADC Clk select
////    AD1CON2bits.VCFG=0b000;     //Vref+=AVDD   Vref-=AVSS
////    AD1CON2bits.CSCNA=0;    //Do not scan inputs; use channel selected by CH0SA bits
////    AD1CON2bits.SMPI=0b0000; //Any ADC interrrups occur at completion of each sample/convert seq.
////    AD1CON2bits.BUFM=0; //Buffer confugured as 1 16 word buffer 
////    AD1CON2bits.ALTS=0; //Always use MUXA input MUX settings
////    AD1CON3bits.ADRC=0; //0: Use System clk; 1: ADC uses internal RC clock for idle or sleep mode
////    AD1CON3bits.SAMC=0; 
////    AD1CON3bits.ADCS=0; //ADCS conversion clock select bits. Ignored if using internal AD RC clock. 
////
////    // MUX settings
//////    AD1CHSbits.CH0NB=0;    //Ch0 MUXB -ve Input is VR-
//////    AD1CHSbits.CH0SB=0bXXXX;    //YOUR ADC CHANNEL HERE
////    AD1CHSbits.CH0NA = 0;    //Ch0 -ve Input is VR-
////    AD1CHSbits.CH0SA = 12;    //YOUR ADC CHANNEL HERE
////
////    //IO port selection
////    TRISBbits.TRISB12 = 1 ; //SET YOUR PIN AS AN INPUT
////    AD1PCFGbits.PCFG12 = 0;     //SET YOUR PIN AS AN ANALOG INPUT
////    AD1CON1bits.ADON=1;
//
//    // Unsigned fractional format, Clear SAMP bit to
//    // start conversion, Sample when SAMP bit is set,
//    // sampling on hold
//
//}


