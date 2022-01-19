
#ifndef IOS_H
#define	IOS_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

void IOinit(); // member function init
void IOcheck(); // implements IO  checks and LED blinking functions
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void);
#endif	/* ends IOS_H */
