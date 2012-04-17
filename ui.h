/* 
 * gPlot++ by Geecko
 *
 * This work is licensed under a Creative Commons license.
 * See LICENSE for more details.
 */

#ifndef UI_H
#define UI_H

#include "functionmanager.h"
#include "controls.h"
#include "common.h"
#include <intraFont.h>
#include <danzeff.h>

#define ELEMENT_H 18

class FunctionManager;

class UI
{
private:
    FunctionManager* manager;
    intraFont *sfont;
    bool active, activated;
    int selected, cursor;
    
    void edit(DanzeffChar c);
    
public:
    UI(FunctionManager* manager, intraFont *sfont);
    ~UI();
    void drawFunctionList();
    void draw();
    void controls(Controls* ctrl);
    bool isActive();
};

#endif
