
#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

uint16_t do_ADC(int pin); // performs ADC init, samples and calls the draw function
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void); // For button interrupt
void voltmeter(void);
void ohmmeter(void);
void capacitance(void);
#endif	/* ends IOS_H */
