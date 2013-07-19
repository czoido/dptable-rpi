CXX=g++
INCLUDES=

UNAME := $(shell uname)

FLAGS=-D__LINUX_ALSA__ -c -fpermissive
LIBS=-lasound -lpthread -lstdc++ -lm $(shell pkg-config ncurses --libs) $(shell pkg-config form --libs)

OBJS=   RtAudio.o chuck_fft.o Thread.o Stk.o dptable.o filter.o table_control.o utils.o twitter_functions.o

dptable: $(OBJS)
	$(CXX) -o dptable $(OBJS) $(LIBS)

dptable.o: dptable.cpp RtAudio.h chuck_fft.h
	$(CXX) $(FLAGS) dptable.cpp

Thread.o: Thread.cpp Thread.h 
	$(CXX) $(FLAGS) Thread.cpp

Stk.o: Stk.h Stk.cpp
	$(CXX) $(FLAGS) Stk.cpp

RtAudio.o: RtAudio.h RtAudio.cpp RtError.h
	$(CXX) $(FLAGS) RtAudio.cpp

chuck_fft.o: chuck_fft.h chuck_fft.c
	$(CXX) $(FLAGS) chuck_fft.c

filter.o: filter.h filter.c
		$(CXX) $(FLAGS) filter.c
		
table_control.o: table_control.h table_control.c
	$(CXX) $(FLAGS) table_control.c


utils.o: utils.h utils.c
	$(CXX) $(FLAGS) utils.c

twitter_functions.o: twitter_functions.h twitter_functions.c
	$(CXX) $(FLAGS) twitter_functions.c

clean:
	rm -f *~ *# *.o dptable
