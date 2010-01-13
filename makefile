
MAKE=make

.PHONY: clean

#minimalistic console for linux\windows
console: 
	$(MAKE)	-f makefiles/makefile.con pbanki.exe

#pocket book device
pb: 
	$(MAKE)	-f makefiles/makefile.pb pbanki.app

#pocket book emulator
pbemu:
	$(MAKE)	-f makefiles/makefile.pbemu pbanki_emu.exe



clean:
	del pbanki_emu.exe
	del pbanki.app
	del obj\pb\*.o
	del obj\pbemu\*.o
      

