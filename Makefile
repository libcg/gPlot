MEDIA = /media/PANDORA/PSP/GAME/CAT_Homebrews\ perso/
TARGET = gPlot++

OBJS = main.o view.o function.o callbacks.o controls.o
       
LIBS = -lglib2d -lpspvram -ljpeg -lpng -lz -lpspgum -lpspgu -lpsprtc -lpspfpu \
       -lmuparser -lm -lstdc++

CFLAGS = -O2 -G0 -Wall -I./include -L./lib
CXXFLAGS = $(CFLAGS) -fexceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = gPlot++

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak 
install:
	cp ./EBOOT.PBP $(MEDIA)$(TARGET)
dir:
	mkdir $(MEDIA)$(TARGET)
lclean:
	rm *.o
