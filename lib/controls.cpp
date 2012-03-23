#include "controls.h"
#include <cstring>

Controls::Controls()
{
    memset(&pad, 0, sizeof(SceCtrlData));
    memset(&oldPad, 0, sizeof(SceCtrlData));
}

void Controls::update()
{
    oldPad = pad;
    sceCtrlPeekBufferPositive(&pad, 1);
}

bool Controls::buttonPressed(PspCtrlButtons btn)
{
    return (pad.Buttons & btn) == btn;
}

bool Controls::buttonWasPressed(PspCtrlButtons btn)
{
    return (oldPad.Buttons & btn) == btn;
}

bool Controls::buttonJustPressed(PspCtrlButtons btn)
{
    return buttonPressed(btn) && !buttonWasPressed(btn);
}

bool Controls::buttonJustReleased(PspCtrlButtons btn)
{
    return !buttonPressed(btn) && buttonWasPressed(btn);
}

SceCtrlData* Controls::getPad()
{
    return &pad;
}

SceCtrlData* Controls::getOldPad()
{
    return &oldPad;
}
