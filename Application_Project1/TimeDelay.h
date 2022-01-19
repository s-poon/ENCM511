#ifndef TIMEDELAY_H
#define	TIMEDELAY_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif
void TimeDelayInit();                                           // Sets up the timer
void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void); // T2 interrupt
void Delay_ms(uint16_t time_ms);                                // delay functions used to time LED blinks
#endif	/* ends IOS_H */

