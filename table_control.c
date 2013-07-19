
#include "table_control.h"
#include "utils.h"

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

dptable _dptable;

int open_connection()
{
    #ifdef __arm__
   //printf("connecting...\n");
    if ((_dptable._i2c_file = open(DEV_I2C, O_RDWR)) < 0) 
    {
        perror("i2c open file fail\n");
        return 1;
    }
   //printf("connected!\n");
    #endif
    return 0;
}

int set_i2c(unsigned char address, unsigned char value)
{
    #ifdef __arm__
    unsigned char outbuf[2];
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];
    messages[0].addr  = address;
    messages[0].flags = 0;
    messages[0].len   = sizeof(outbuf);
    messages[0].buf   = outbuf;
    outbuf[0] = value;
    outbuf[1] = value;
    packets.msgs  = messages;
    packets.nmsgs = 1;
    if(ioctl(_dptable._i2c_file, I2C_RDWR, &packets) < 0) {
        perror("error sending data\n");
        return 1;
    }
    #endif
    return 0;   
}

int load_sequence_file(char* file)
{
    char line[32];
    FILE *file_;
    file_ = fopen(file, "r" );
    int row=0;
    int num_sequence = -1;
    int num_grid = 0;
    _dptable._num_sequences = 0;
    _dptable._current_seq = 0;
    while (fgets(line,16, file_)!=NULL)
    {
        if (strchr(line,'-')!=NULL)
        {
            row=0;
            num_grid++;
            _dptable._list_sequences[num_sequence]._num_grids++;
        }
        else if (strchr(line,'#')!=NULL)
        {
            num_sequence++;
            _dptable._num_sequences = num_sequence+1;
            num_grid = 0;
            strncpy(_dptable._list_sequences[num_sequence]._name,line,strlen(line)-1);
            _dptable._list_sequences[num_sequence]._num_grids = 0;
            _dptable._list_sequences[num_sequence]._current_grid_num = 0;
        }
        else if(strlen(line)>3)
        {
            for (int col=0;col<5;col++)
                _dptable._list_sequences[num_sequence]._sequence[num_grid]._cell[row][col]=line[col]-'0';

            row++;
        }
    }
    fclose(file_);    
    return 0;
}

int push_current_grid()
{
    //first PCF8574 -> 112 address, just one led on pos 00000001
    char bit_str[16];
    sprintf(bit_str,"0000000%d",_dptable._current_grid._cell[0][4]);
    int value = bin2dec(bit_str,-1);
    set_i2c(56, value);
    //second PCF8574 -> 114 address, eight leds 1,4 2,4 3,4 4,4 4,3 3,3 2,3 1,3
    sprintf(bit_str,"%d%d%d%d%d%d%d%d",_dptable._current_grid._cell[1][4]
                                      ,_dptable._current_grid._cell[2][4]
                                      ,_dptable._current_grid._cell[3][4]
                                      ,_dptable._current_grid._cell[4][4]
                                      ,_dptable._current_grid._cell[4][3]
                                      ,_dptable._current_grid._cell[3][3]
                                      ,_dptable._current_grid._cell[2][3]
                                      ,_dptable._current_grid._cell[1][3]);
    value = bin2dec(bit_str,1);
    set_i2c(57, value);
    //third PCF8574 -> 116 address, eight leds 0,3 0,2 1,2 2,2 3,2 4,2 4,1 3,1
    sprintf(bit_str,"%d%d%d%d%d%d%d%d",_dptable._current_grid._cell[0][3]
                                      ,_dptable._current_grid._cell[0][2]
                                      ,_dptable._current_grid._cell[1][2]
                                      ,_dptable._current_grid._cell[2][2]
                                      ,_dptable._current_grid._cell[3][2]
                                      ,_dptable._current_grid._cell[4][2]
                                      ,_dptable._current_grid._cell[4][1]
                                      ,_dptable._current_grid._cell[3][1]);
    value = bin2dec(bit_str,1);
    set_i2c(58, value);
    //fourth PCF8574 -> 118 address, eight leds 2,1 1,1 0,1 0,0 1,0 2,0 3,0 4,0
    sprintf(bit_str,"%d%d%d%d%d%d%d%d",_dptable._current_grid._cell[2][1]
                                      ,_dptable._current_grid._cell[1][1]
                                      ,_dptable._current_grid._cell[0][1]
                                      ,_dptable._current_grid._cell[0][0]
                                      ,_dptable._current_grid._cell[1][0]
                                      ,_dptable._current_grid._cell[2][0]
                                      ,_dptable._current_grid._cell[3][0]
                                      ,_dptable._current_grid._cell[4][0]);
    value = bin2dec(bit_str,1);
    set_i2c(59, value);
    return 0;
}

void show_letter(int print_letter)
{
    int ascii_code = print_letter;
    int index = 5*(ascii_code-32);
   //printf("ascii:%d index: %d",ascii_code,index);
    unsigned char letter[5][5];
    for (int row=0;row<5;row++)
    {
        unsigned char c0 = (font[index+(4-row)]);c0=c0>>3;
        unsigned char c1 = (font[index+(4-row)]);c1=c1>>3;
        unsigned char c2 = (font[index+(4-row)]);c2=c2>>3;
        unsigned char c3 = (font[index+(4-row)]);c3=c3>>3;
        unsigned char c4 = (font[index+(4-row)]);c4=c4>>3;
        
        unsigned char c0_ = 16&c0;c0_=c0_>>4;
        unsigned char c1_ = 8&c1;c1_=c1_>>3;
        unsigned char c2_ = 4&c2;c2_=c2_>>2;
        unsigned char c3_ = 2&c3;c3_=c3_>>1;
        unsigned char c4_ = 1&c4;
            
        letter[row][0]=c0_;		letter[row][1]=c1_;
        letter[row][2]=c2_;		letter[row][3]=c3_;
        letter[row][4]=c4_;
    }

    for (int col=0;col<5;col++)
    {
        for (int row=0;row<5;row++)
        {
            _dptable._current_grid._cell[row][col]=letter[row][col];
        }
    }
    push_current_grid();
        
}


int play_delay_sequence(char* name,int ms_delay)
{
    for (int j=0;j<_dptable._num_sequences;j++)
    {
        int result = strcmp(name,_dptable._list_sequences[j]._name);
        if (result==0)
        {
            _dptable._current_seq=j;
           //printf("playing %s with delay: %d ms\n",_dptable._list_sequences[j]._name,ms_delay);
            for (int i=0;i<_dptable._list_sequences[j]._num_grids;i++)
            {
                for (int r=0;r<5;r++)
                {
                    for (int c=0;c<5;c++)
                    {
                       //printf("%d",_dptable._list_sequences[j]._sequence[i]._cell[r][c]);
                        _dptable._current_grid._cell[r][c]=_dptable._list_sequences[j]._sequence[i]._cell[r][c];
                    }
                   //printf("\n");
                }
               //printf("-----\n");
                push_current_grid();
                _dptable._list_sequences[j]._current_grid_num++;
                usleep(1000*ms_delay);
            }
        }
    }
    return 0;
}

int set_cells(char* cells)
{
    _dptable._current_grid._cell[0][0]=cells[0]-'0';
    _dptable._current_grid._cell[0][1]=cells[1]-'0';
    _dptable._current_grid._cell[0][2]=cells[2]-'0';
    _dptable._current_grid._cell[0][3]=cells[3]-'0';
    _dptable._current_grid._cell[0][4]=cells[4]-'0';

    _dptable._current_grid._cell[1][0]=cells[5]-'0';
    _dptable._current_grid._cell[1][1]=cells[6]-'0';
    _dptable._current_grid._cell[1][2]=cells[7]-'0';
    _dptable._current_grid._cell[1][3]=cells[8]-'0';
    _dptable._current_grid._cell[1][4]=cells[9]-'0';

    _dptable._current_grid._cell[2][0]=cells[10]-'0';
    _dptable._current_grid._cell[2][1]=cells[11]-'0';
    _dptable._current_grid._cell[2][2]=cells[12]-'0';
    _dptable._current_grid._cell[2][3]=cells[13]-'0';
    _dptable._current_grid._cell[2][4]=cells[14]-'0';

    _dptable._current_grid._cell[3][0]=cells[15]-'0';
    _dptable._current_grid._cell[3][1]=cells[16]-'0';
    _dptable._current_grid._cell[3][2]=cells[17]-'0';
    _dptable._current_grid._cell[3][3]=cells[18]-'0';
    _dptable._current_grid._cell[3][4]=cells[19]-'0';
    
    _dptable._current_grid._cell[4][0]=cells[20]-'0';
    _dptable._current_grid._cell[4][1]=cells[21]-'0';
    _dptable._current_grid._cell[4][2]=cells[22]-'0';
    _dptable._current_grid._cell[4][3]=cells[23]-'0';
    _dptable._current_grid._cell[4][4]=cells[24]-'0';
    
    push_current_grid();
    return 0;
}

int next_grid()
{
    int seq = _dptable._current_seq;
   //printf("current seq: %",seq);
    for (int r=0;r<5;r++)
    {
        for (int c=0;c<5;c++)
        {
            int grid = _dptable._list_sequences[seq]._current_grid_num;
           //printf("%d",_dptable._list_sequences[seq]._sequence[grid]._cell[r][c]);
            _dptable._current_grid._cell[r][c]=_dptable._list_sequences[seq]._sequence[grid]._cell[r][c];
        }
       //printf("\n");
    }
   //printf("-----\n");
    push_current_grid();
    usleep(5000);
   //printf("current grid: %",_dptable._list_sequences[seq]._current_grid_num);
    _dptable._list_sequences[seq]._current_grid_num++;
    if (_dptable._list_sequences[seq]._current_grid_num>_dptable._list_sequences[seq]._num_grids)
        _dptable._list_sequences[seq]._current_grid_num=0;
    return 0;
}

int select_sequence(char* name)
{
    for (int j=0;j<_dptable._num_sequences;j++)
    {
        int result = strcmp(name,_dptable._list_sequences[j]._name);
        if (result==0)
        {
           //printf("sequence selected: %s\n",_dptable._list_sequences[j]._name);
            _dptable._current_seq=j;
            _dptable._list_sequences[_dptable._current_seq]._current_grid_num=0;
        }
    }
    return 0;
}

int scroll_message(char* message, int ms_delay)
{
    int len_msg = strlen(message)-1;
    int* scroll[5];
    for (int r=0;r<5;r++)
        scroll[r] = (int*)calloc((5+len_msg*5+len_msg),sizeof(int));
    
    int final_column=0;
    for(int k = 0; k < len_msg; k++)
    {
        int ascii_code = message[k];
        int index = 5*(ascii_code-32);
       //printf("ascii:%d index: %d",ascii_code,index);
        unsigned char letter[5][5];
        for (int row=0;row<5;row++)
        {
            unsigned char c0 = (font[index+(4-row)]);c0=c0>>3;
            unsigned char c1 = (font[index+(4-row)]);c1=c1>>3;
            unsigned char c2 = (font[index+(4-row)]);c2=c2>>3;
            unsigned char c3 = (font[index+(4-row)]);c3=c3>>3;
            unsigned char c4 = (font[index+(4-row)]);c4=c4>>3;

            unsigned char c0_ = 16&c0;c0_=c0_>>4;
            unsigned char c1_ = 8&c1;c1_=c1_>>3;
            unsigned char c2_ = 4&c2;c2_=c2_>>2;
            unsigned char c3_ = 2&c3;c3_=c3_>>1;
            unsigned char c4_ = 1&c4;

            letter[row][0]=c0_;		letter[row][1]=c1_;
            letter[row][2]=c2_;		letter[row][3]=c3_;
            letter[row][4]=c4_;
        }
        for (int col=0;col<5;col++)
        {
            for (int row=0;row<5;row++)
            {
                scroll[row][final_column]=letter[row][col];
            }
            final_column++;
        }
        //letter separation
        for (int row=0;row<5;row++)
        {
            scroll[row][final_column]=0;
        }
        final_column++;
    }    

    for (int index=0;index<len_msg*5+len_msg;index++)
    {
        for (int col=0;col<5;col++)
        {
            for (int row=0;row<5;row++)
            {
                _dptable._current_grid._cell[row][col]=scroll[row][index+col];
            }
        }
        push_current_grid();
        usleep(1000*ms_delay);
    }
    
    for (int r=0;r<5;r++)
        free(scroll[r]);
}

int print_date()
{
    char chr_time[256];
    time_t mytime;
    mytime = time(NULL);
    sprintf(chr_time,"%s",ctime(&mytime));
    scroll_message(chr_time,133);
}

int close_connection()
{
    close(_dptable._i2c_file);
    return 0;
}
