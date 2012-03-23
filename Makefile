MEDIA = /media/PANDORA/PSP/GAME/CAT_Homebrews\ perso/
TARGET = gPlot++

OBJS = main.o view.o                                                           \
       lib/callbacks.o lib/controls.o lib/glib2d.o
LIBS = -lpspvram -ljpeg -lpng -lz -lpspgum -lpspgu -lpsprtc -lm -lpspfpu       \
       -lstdc++

CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
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
