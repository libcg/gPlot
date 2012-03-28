MEDIA = /media/PANDORA/PSP/GAME/CAT_Homebrews\ perso/
TARGET = gPlot++

OBJS = main.o view.o function.o functionmanager.o callbacks.o controls.o
       
LIBS = -lglib2d -lpspvram -ljpeg -lpng -lz -lpspgum -lpspgu -lpsprtc -lpspfpu \
       -lmuparser -lm -lpsppower -lstdc++

CFLAGS = -O2 -G0 -Wall -I./include -L./lib -g
CXXFLAGS = $(CFLAGS) -fexceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 0
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = gPlot++
PSP_FW_VERSION = 500

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 
install:
	cp ./EBOOT.PBP $(MEDIA)$(TARGET)
dir:
	mkdir $(MEDIA)$(TARGET)
lclean:
	rm *.o
