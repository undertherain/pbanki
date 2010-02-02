
MAKE=make

.PHONY: clean release

#minimalistic console for linux\windows
console: 
	$(MAKE)	-f makefiles/makefile.con pbanki.exe

#pocket book device
pb: 
	$(MAKE)	-f makefiles/makefile.pb mindcraft.app

#pocket book emulator
pbemu:
	$(MAKE)	-f makefiles/makefile.pbemu mindcraft.exe

#OpenInkpot (for now for lbook)
oi:
	$(MAKE)	-f makefiles/makefile.oi mindcraft

oiarm:
	$(MAKE)	-f makefiles/makefile.oi.arm mindcraft
	
#qt linux
qt:
	$(MAKE) -f makefiles/makefile.qt mindcraft

qtoi: 
	$(MAKE) -f makefiles/makefile.qtoi mindcraft

clean:
	del pbanki_emu.exe
	del pbanki.app
	del obj\pb\*.o
	del obj\pbemu\*.o

release: pb
	echo obj\a.exe
