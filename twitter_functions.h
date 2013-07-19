/* 
 * File:   twetter_functions.h
 * Author: carlos
 *
 * Created on 6 de diciembre de 2012, 12:50
 */

#ifndef TWETTER_FUNCTIONS_H
#define	TWETTER_FUNCTIONS_H

#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "table_control.h"

#ifdef	__cplusplus
extern "C" {
#endif

    void tweet_hello_table();
    void check_new_mentions();


#ifdef	__cplusplus
}
#endif

#endif	/* TWETTER_FUNCTIONS_H */

