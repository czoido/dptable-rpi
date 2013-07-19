//-----------------------------------------------------------------------------
// description: main file for the daft punk table control program
// author: Carlos Zoido
// based on the code from Jorge Herrera https://ccrma.stanford.edu/courses/256a/lectures/5b/fft/
// some code for the server: https://en.wikipedia.org/wiki/Berkeley_sockets
// date: 18.07.2013
//-----------------------------------------------------------------------------

#include "RtAudio.h"
#include "chuck_fft.h"
#include "Thread.h"
#include "utils.h"
#include "filter.h"
#include "table_control.h"
#include "twitter_functions.h"

#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <ncurses.h>
#include <form.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define MY_SRATE 44100
#define SAMPLE float
#define ZPF 1

bool g_process_music = true;

Mutex g_mutex;

float * g_fftBuff;
float * g_window;
bool g_useWindow = false;
bool g_displayBars = false;
unsigned int g_buffSize = 4096;
int time_after_last_setcell = 0;
void calcFFT() {
    if (g_process_music)
    {
        complex * fft = (complex *) g_fftBuff;
        if (g_fftBuff) {
            g_mutex.lock();
			time_after_last_setcell++;
            //printf("%d\n",time_after_last_setcell);
			if (time_after_last_setcell>900000)
	        {
				analyze_buffer(fft, ZPF*g_buffSize, MY_SRATE);
			}
            g_mutex.unlock();

        }
    }
}



int audioCallback(void * outputBuffer, void * inputBuffer,
        unsigned int bufferSize, double streamTime,
        RtAudioStreamStatus status, void * userData) {
    SAMPLE * in = (SAMPLE *) inputBuffer;

    g_mutex.lock();

    memset(g_fftBuff, 0, ZPF * bufferSize * sizeof (float));

    for (size_t i = 0; i < bufferSize; ++i) {
        g_fftBuff[i] = in[i];
    }

    // apply window to the buffer of audio
    if (g_useWindow)
        apply_window(g_fftBuff, g_window, g_buffSize);

    // compute the fft
    rfft(g_fftBuff, ZPF * bufferSize / 2, FFT_FORWARD);

    g_mutex.unlock();

    return 0;
}

int split_string(const std::string& input, const std::string& delimiter, std::vector<std::string>& results) {
    int iPos = 0;
    int newPos = -1;
    int sizeS2 = delimiter.size();
    int isize = input.size();

    std::vector<int> positions;

    newPos = input.find(delimiter, 0);

    if (newPos < 0) {
        return 0;
    }

    int numFound = 0;

    while (newPos > iPos) {
        numFound++;
        positions.push_back(newPos);
        iPos = newPos;
        newPos = input.find(delimiter, iPos + sizeS2 + 1);
    }

    for (int i = 0; i <= positions.size(); i++) {
        std::string s;
        if (i == 0) {
            s = input.substr(i, positions[i]);
        }
        int offset = -100;
        if (i > 0)
            offset = positions[i - 1] + sizeS2;
        if (offset < isize) {
            if (i == positions.size()) {
                s = input.substr(offset);
            } else if (i > 0) {
                s = input.substr(positions[i - 1] + sizeS2,
                        positions[i] - positions[i - 1] - sizeS2);
            }
        }
        if (s.size() > 0) {
            results.push_back(s);
        }
    }
    return numFound;
}


void send_action(std::string action) {
    g_mutex.lock();
    std::vector<std::string> results;
    split_string(action, ":", results);
	action = results[0];
	std::cout << "action: " << action << std::endl;
	if (action=="play")
	{
		std::string argument = results[1];
		int ms = atoi(results[2].c_str());
		std::cout << "argument: " << argument << std::endl;
		std::cout << "time: " << ms << std::endl;
		char seq[256];
		sprintf(seq,"#%s",argument.c_str());
		play_delay_sequence(seq,ms);
		select_sequence("#normal");
	}
	if (action=="twitter")
	{
		check_new_mentions();
	}
	if (action=="setcells")
	{
		set_cells(results[1].c_str());
		time_after_last_setcell = 0;
	}
	if (action=="scroll")
	{
		std::cout << "argument: " << results[1] << std::endl;
		scroll_message(results[1].c_str(),233);
		select_sequence("#normal");
	}
	if (action=="gain")
	{
		std::cout << "band: " << results[1] << std::endl;
		std::cout << "gain: " << results[2] << std::endl;
		int band = atoi(results[1].c_str());
		float gain = atof(results[2].c_str());
		set_band_gain(band,gain);
	}
    g_mutex.unlock();
}


#define PORTNUM 8888

void *start_server() {
    char buf[1024];
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == SocketFD) {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }

    int so_reuseaddr = 1;

    int z = setsockopt(SocketFD,
            SOL_SOCKET,
            SO_REUSEADDR,
            &so_reuseaddr,
            sizeof so_reuseaddr);

    memset(&stSockAddr, 0, sizeof (stSockAddr));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(8888);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;

    if (-1 == bind(SocketFD, (struct sockaddr *) &stSockAddr, sizeof (stSockAddr))) {
        perror("error bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if (-1 == listen(SocketFD, 10)) {
        perror("error listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    for (;;) {
        int ConnectFD = accept(SocketFD, NULL, NULL);

        if (0 > ConnectFD) {
            perror("error accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }

        /* perform read write operations ...
        read(ConnectFD,buff,size)*/
        int n = 0;
        while (n = recv(ConnectFD, buf, 1024, 0)) {
            buf[n] = '\0';
            send_action(buf);
        }
        if (-1 == shutdown(ConnectFD, SHUT_RDWR)) {
            perror("can not shutdown socket");
            close(ConnectFD);
            close(SocketFD);
            exit(EXIT_FAILURE);
        }
        close(ConnectFD);
    }

    close(SocketFD);
}

int main(int argc, char *argv[]) {

    pthread_t thread;
    int iret;
    iret = pthread_create(&thread, NULL, start_server, NULL);

    char print_letter[256] = "NONE";
    if (argc > 2) {
        sprintf(print_letter, "%s\n", argv[2]);
    }

    // pointer to RtAudio object
    RtAudio * audio = NULL;
    RtAudio::DeviceInfo info;
    // create the object
    try {
        audio = new RtAudio();
    } catch (RtError & err) {
        //err.printMessage();
        exit(1);
    }

    unsigned int devices = audio->getDeviceCount();
    std::cout << "\nFound " << devices << " device(s) ...\n";

    for (unsigned int i = 0; i < devices; i++) {
        info = audio->getDeviceInfo(i);
        std::cout << "\ndevice Name = " << info.name << '\n';
    }

    if (audio->getDeviceCount() < 1) {
        // nopes
        cout << "no audio devices found!" << endl;
        exit(1);
    }

    // let RtAudio print messages to stderr.
    audio->showWarnings(true);

    // set input and output parameters
    RtAudio::StreamParameters iParams;
    iParams.deviceId = audio->getDefaultInputDevice();
    iParams.nChannels = 1;
    iParams.firstChannel = 0;

    // create stream options
    RtAudio::StreamOptions options;

    // set the callback and start stream
    try {
        audio->openStream(NULL, &iParams, RTAUDIO_FLOAT32, MY_SRATE, &g_buffSize, &audioCallback, NULL, &options);

        cerr << "Buffer size defined by RtAudio: " << g_buffSize << endl;

        // allocate the buffer for the fft
        g_fftBuff = new float[g_buffSize * ZPF];
        if (g_fftBuff == NULL) {
            cerr << "Something went wrong when creating the fft buffers" << endl;
            exit(1);
        }

        // allocate the buffer for the time domain window
        g_window = new float[g_buffSize];
        if (g_window == NULL) {
            cerr << "Something went wrong when creating the window" << endl;
            exit(1);
        }

        // create a hanning window
        make_window(g_window, g_buffSize);

        // start the audio stream
        audio->startStream();

        // test RtAudio functionality for reporting latency.
        cout << "stream latency: " << audio->getStreamLatency() << " frames" << endl;
    } catch (RtError & err) {
        //err.printMessage();
        goto cleanup;
    }

    open_connection();
    load_sequence_file("seq.dat");
    if (strcmp(print_letter, "NONE") == 0) {
        //play_delay_sequence("#init",300);
        //play_delay_sequence("#beat",100);
        //play_delay_sequence("#normal",500);
    } else {
        //play_delay_sequence("#init",300);
        //printf("show letter %d\n",print_letter[0]);
        if (strlen(print_letter) == 2) {
            show_letter(print_letter[0]);
        } else
            scroll_message(print_letter, 300);
    }

    tweet_hello_table();
    check_new_mentions();
    play_delay_sequence("#init",400);
    select_sequence("#normal");
    load_filter("filter.dat");

    int ch;

    while (1) {
        calcFFT();
    }

    try {
        audio->stopStream();
    } catch (RtError & err) {
        err.printMessage();
    }

    // Clean up
cleanup:
    if (audio) {
        audio->closeStream();
        delete audio;
    }


    return 0;
}

