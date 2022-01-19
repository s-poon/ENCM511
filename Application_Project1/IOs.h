
#ifndef IOS_H
#define	IOS_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

void IOinit();                                                      // member function init
void IOcheck();                                                     // implements IO  checks and LED blinking functions
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void);     // CN interrupt function
void draw_graph(uint16_t ADCvalue);                                 // Function to draw the graw
uint16_t do_ADC(void);                                              // Function to take an ADC Sample
#endif	/* ends IOS_H */
