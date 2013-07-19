#include "filter.h"
#include "table_control.h"
#include <ncurses.h>

filtro _filter;

void set_band_gain(int band, float gain)
{
	if (band<_filter._num_bands)
	{
		_filter.band[band].factor = gain;
	}
}

int load_filter(char* filter_file)
{
    _filter._num_bands=0;
    char line[16];
    FILE *file_;
    file_ = fopen(filter_file,"r" );
    int index = 0;
    int num_band = 0;
    while (fgets(line,16, file_)!=NULL)
    {
        if(strlen(line)>1)
        {
            if (index%3==0)
            {
                _filter._num_bands++;
                _filter.band[num_band]._max_history_size = 200;
                _filter.band[num_band]._history_size = 0;
                _filter.band[num_band].factor = atof(line);
                _filter.band[num_band].max_rep = 0;
                _filter.band[num_band].min_rep = 0;
                for (int i=0;i<_filter.band[num_band]._max_history_size;i++)
                    _filter.band[num_band]._history.push_back(0);
                
                printf("%f ",_filter.band[num_band].factor);
            }
            if (index%3==1)
            {
                _filter.band[num_band]._min_freq = atoi(line);
                printf("%d ",_filter.band[num_band]._min_freq);
            }
            if (index%3==2)
            {
                _filter.band[num_band]._max_freq = atoi(line);
                printf("%d\n",_filter.band[num_band]._min_freq);
                num_band++;
                if (num_band>BANDS_MAX)
                    break;
            }
            index++;
        }
    }
    fclose(file_);
    return 1;
}

int freq_to_index(float freq,int buffer_size, int rate)
{
    return (freq*(buffer_size/2.0))/(float)rate;
}

int aux = 0;
int wait = 0;

void analyze_buffer(complex *cbuf, int buff_size, int rate)
{
    for(int i = 0;i<_filter._num_bands;i++)
    {
        _filter.band[i].value=0;
        _filter.band[i].average=0;
    }
    for (int ba=0;ba<_filter._num_bands;ba++)
    {
        int ind_min = freq_to_index(_filter.band[ba]._min_freq,buff_size,rate);
        int ind_max = freq_to_index(_filter.band[ba]._max_freq,buff_size,rate);
        for(int i = ind_min; i < ind_max; i++ )
        {
            float amp = (cbuf[i].re*cbuf[i].re + cbuf[i].im*cbuf[i].im);
            float power = amp*amp;
            _filter.band[ba].value=_filter.band[ba].value+power;
        }
        _filter.band[ba]._history.push_back(_filter.band[ba].value/_filter.band[ba]._max_history_size);
        _filter.band[ba]._history.pop_front();
    }
    for (int i=0;i<_filter._num_bands;i++)
    {   
        std::deque<float>::iterator iter;        
        for (iter = _filter.band[i]._history.begin(); iter != _filter.band[i]._history.end(); ++iter)
        {
            _filter.band[i].average = (*iter) + _filter.band[i].average;
        }
    }
    aux++;
    if (aux%50==0)
    {
        bool is_beat = false;
        for (int i=0;i<_filter._num_bands;i++)
        {
            if (_filter.band[i].value>_filter.band[i].factor*_filter.band[i].average && _filter.band[i].average>0.001)
            {
                is_beat = true;
                if (wait>1000)
                {
					next_grid();
					wait=0;
                }
            }
        }
    }
    wait++;
}
