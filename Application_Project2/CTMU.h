/* 
 * File:   CTMU.h
 * Author: Me
 *
 * Created on 10 December 2021, 00:10
 */

#ifndef CTMU_H
#define	CTMU_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

//uint16_t CTMU(void);
void CTMUInit(void);
void ADC1_for_CTMU_init(void);
float do_CTMU(void);
#endif	/* CTMU_H */

