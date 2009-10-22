#compiling for pocketbook arm arch

CC = $(POCKETBOOKSDK)\arm-linux\bin\gcc
CPP = $(POCKETBOOKSDK)\arm-linux\bin\g++
STRIP = $(POCKETBOOKSDK)\arm-linux\bin\strip

SOURCES = src/main_pocketbook.cpp src/include/deck.cpp src/include/deckAnki.cpp src/include/model.cpp src/include/view_pb.cpp  src/include/logger.cpp src/include/dbSQLite.cpp
INCLUDE=-I/arm-linux/include
PATH=$(POCKETBOOKSDK)\arm-linux\bin;$(POCKETBOOKSDK)\bin
LIBS=-linkview -lfreetype -ljpeg -lz -pthread


pbanki.app: sqlite3.o
	$(CPP) -Wall -O2 -fomit-frame-pointer $(INCLUDE) sqlite3.o -o $@ $(LIBS) $(SOURCES)
	$(STRIP) pbanki.app


sqlite3.o:
	$(CC) -c src/include/libs/sqlite3.c


	
	