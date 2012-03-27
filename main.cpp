#include "main.h"
#include "callbacks.h"
#include <glib2d.h>
#include <pspkernel.h>
#include <pspfpu.h>
#include <psppower.h>

PSP_MODULE_INFO("app", 0, 1, 1);
PSP_HEAP_SIZE_KB(-4096);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

Main::Main() :
    ctrl(), manager(), view(&manager) 
{
    g2dInit();
    
    pspFpuSetEnable(0);
    scePowerSetClockFrequency(333, 333, 166);
}

Main::~Main()
{
    g2dTerm();
}

int Main::run()
{
    while (1)
    {
        ctrl.update();
        view.controls(&ctrl);
        
        g2dClear(WHITE);
        {
            view.draw();
        }
        g2dFlip(G2D_VSYNC);
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    callbacks_setup();
    
    Main main;
    
    return main.run();
}
