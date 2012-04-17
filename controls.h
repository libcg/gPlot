/* 
 * gPlot++ by Geecko
 *
 * This work is licensed under a Creative Commons license.
 * See LICENSE for more details.
 */

#ifndef CTRL_H
#define CTRL_H

#include <pspctrl.h>

class Controls
{
private:
    SceCtrlData pad;
    SceCtrlData oldPad;
    
public:
    Controls();
    void update();
    bool buttonPressed(PspCtrlButtons btn);
    bool buttonWasPressed(PspCtrlButtons btn);
    bool buttonJustPressed(PspCtrlButtons btn);
    bool buttonJustReleased(PspCtrlButtons btn);
    SceCtrlData* getPad();
    SceCtrlData* getOldPad();
};

#endif
