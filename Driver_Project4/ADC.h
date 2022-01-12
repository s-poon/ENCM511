
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

uint16_t do_ADC(void); // performs ADC init, samples and calls the draw function
void draw_graph(uint16_t ADCvalue); // draws the bar graph onto the terminal
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void); // For button interrupt
#endif	/* ends IOS_H */
