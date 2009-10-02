CC = g++

PATH=$(POCKETBOOKSDK);$(POCKETBOOKSDK)\bin
#damn!!! 	c:\bin\MinGW\bin\g++ a.cpp  - works
#		c:\bin\PBSDK\bin\g++ a.cpp  - does not
#only if PATH is set to c:\bin\PBSDK\bin
EMUCC = g++

SOURCES_CONSOLE = src/main_console.cpp src/include/deck.cpp src/include/model.cpp src/include/view_console.cpp
SOURCES_PB = src/main_pocketbook.cpp src/include/deck.cpp src/include/model.cpp src/include/view_pb.cpp
LIBS_PB = -linkview -lfreetype -ljpeg -lz -lgdi32

win: pbanki_win.exe

pocketbook: 

pocketbook_emu: pbanki_emu.exe




pbanki_win.exe: $(SOURCES_CONSOLE)
	$(CC) $^ -o $@  

pbanki_emu.exe: $(SOURCES_PB)
	$(EMUCC) $(LIBS_PB) $^ -o $@  
      
