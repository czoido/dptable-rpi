/* 
 * File:   table_control.h
 * Author: czoido
 *
 * Created on 28 de noviembre de 2012, 13:01
 */

#ifndef TABLE_CONTROL_H
#define	TABLE_CONTROL_H

#include "font.h"

#define DEV_I2C "/dev/i2c-1"

struct grid
{
    int _cell[5][5];    
};

struct sequence
{
    grid _sequence[256];
    char _name[256];
    int _num_grids;
    int _current_grid_num;
};


struct dptable
{
    int _i2c_file;
    grid _current_grid;
    sequence _list_sequences[16];
    int _num_sequences;
    int _current_seq;
};

#ifdef	__cplusplus
extern "C" {
#endif

    int open_connection();
    int set_i2c(unsigned char address, unsigned char value);
    int close_connection();
    int load_sequence_file(char* file);
    int play_delay_sequence(char* name,int ms_delay);
    int push_current_grid();
    int next_grid();
    int select_sequence(char* name);
    int scroll_message(char* message, int ms_delay);
    int print_date();
    void show_letter(int print_letter);
    int set_cells(char* cells);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TABLE_CONTROL_H */

