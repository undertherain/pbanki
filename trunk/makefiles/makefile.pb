#compiling for pocketbook arm arch

CC = $(POCKETBOOKSDK)~/opt/PBSDK/arm-linux/bin/gcc
CPP = $(POCKETBOOKSDK)~/opt/PBSDK/arm-linux/bin/g++
STRIP = $(POCKETBOOKSDK)~/opt/PBSDK/arm-linux/bin/strip

INCLUDE=-I~opt/PBSDK/arm-linux/include
PATH=$(POCKETBOOKSDK)\arm-linux\bin;$(POCKETBOOKSDK)\bin
LIBS=-linkview -lfreetype -ljpeg -lz -pthread
CFLAGS=-Wall -O2 -fomit-frame-pointer -DARCH_PB
SQLITEFLAGS= -DSQLITE_MIXED_ENDIAN_64BIT_FLOAT 

OBJPATH=obj/pb


include makefiles/makefile.inkview



