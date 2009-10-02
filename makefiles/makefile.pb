#compiling for pocketbook arm arch

CC = $(POCKETBOOKSDK)\arm-linux\bin\g++
SOURCES = src/main_pocketbook.cpp src/include/deck.cpp src/include/model.cpp src/include/view_pb.cpp
INCLUDE=-I/arm-linux/include
PATH=$(POCKETBOOKSDK)\arm-linux\bin;$(POCKETBOOKSDK)\bin
LIBS=-linkview -lfreetype -ljpeg -lz


pbanki.app:
	$(CC) -Wall -O2 -fomit-frame-pointer $(INCLUDE) -o $@ $(LIBS) $(SOURCES)

	
	