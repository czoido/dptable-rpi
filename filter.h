/* 
 * File:   filter.h
 * Author: czoido
 *
 * Created on 28 de noviembre de 2012, 18:38
 */

#ifndef FILTER_H
#define	FILTER_H

#include <deque>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "chuck_fft.h"

#define BANDS_MAX 8

struct band_history
{
    std::deque<float> _history;
    int _min_freq;
    int _max_freq;
    float factor;
    int _history_size;
    int _max_history_size;

    float min_rep;
    float max_rep;
    float value;
    float average;
};

struct filtro
{
    band_history band[BANDS_MAX];
    int _num_bands;
};

#ifdef	__cplusplus
extern "C" {
#endif
    
int load_filter(char* filter_file);    
void set_band_gain(int band, float gain);
int freq_to_index(float freq,int buffer_size, int rate);
void analyze_buffer(complex *cbuf, int buff_size, int rate);

#ifdef	__cplusplus
}
#endif

#endif	/* FILTER_H */

