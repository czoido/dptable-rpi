/* 
 * File:   utils.h
 * Author: czoido
 *
 * Created on 28 de noviembre de 2012, 13:23
 */

#ifndef UTILS_H
#define	UTILS_H

#include <string.h>
#include <stdio.h>
#include <sys/time.h>


#ifdef	__cplusplus
extern "C" {
#endif
   
int bin2dec(char *bin, int reverse);
double elapsed_ms(timeval start, timeval end);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

