#include "ui.h"
#include <string>

UI::UI(FunctionManager* manager, intraFont *sfont) :
    manager(manager), sfont(sfont),
    active(false), selected(0)
{
    danzeffInit();
    danzeffSetMode(DANZEFF_NUMBERS);
    danzeffSetModeLock(false);
}

UI::~UI()
{
    danzeffTerm();
}

void UI::edit(DanzeffChar c)
{
    std::string str = manager->getFunction(selected)->getExpr();
    str = str.substr(0, str.size()-1);

    switch (c)
    {
        case '\010': // Del
            str = str.substr(0, str.size()-1);
            break;
            
        case DANZEFF_LEFT:
        case DANZEFF_RIGHT:
        case DANZEFF_SELECT:
        case DANZEFF_START:
        case '\0':
            return; // Do nothing
            
        default: // ASCII char
            str.push_back((char)c);
            break;                
    }
    
    manager->setFunction(selected, str);
}

void UI::drawFunctionList()
{
    for (unsigned int i=0; i<manager->size(); i++)
    {        
        // Background
        g2dBeginQuads(NULL);
        {
            g2dSetCoordXY(0, i * ELEMENT_H);
            
            // Background
            g2dPush();
            g2dSetColor(WHITE);
            g2dAdd();
            g2dSetCoordXYRelative(G2D_SCR_W, 0);
            g2dAdd();
            g2dSetCoordXYRelative(0, ELEMENT_H);
            g2dSetColor(LITEGRAY);
            g2dAdd();
            g2dSetCoordXYRelative(-G2D_SCR_W, 0);
            g2dAdd();
            g2dPop();
            
            // Color indicator
            static int c=0;
            if ((c += 2) > 255) c = 0;

            g2dPush();
            g2dSetColor(F_COLOR(i));
            g2dSetAlpha((int)i == selected ? c : 255);
            g2dAdd();
            g2dSetCoordXYRelative(10, 0);
            g2dAdd();
            g2dSetCoordXYRelative(-6, ELEMENT_H);
            g2dAdd();
            g2dSetCoordXYRelative(-4, 0);
            g2dAdd();
            g2dPop();
        }
        g2dEnd();
        
        // Expression
        Function* f = manager->getFunction(i);
        std::string str = f->getExpr();
        intraFontSetStyle(sfont, 0.9f, (f->isValid() ? BLACK : RED), 0, 0.f,
                          INTRAFONT_ALIGN_LEFT);
        intraFontPrint(sfont, 10, i * ELEMENT_H + 13, str.c_str());
    }
}

void UI::draw()
{
    if (!active) return;
    
    g2dBeginRects(NULL);
    {
        g2dSetColor(BLACK);
        g2dSetAlpha(255/3);
        g2dSetCoordXY(0, 0);
        g2dSetScaleWH(G2D_SCR_W, G2D_SCR_H);
        g2dAdd();
    }
    g2dEnd();
    
    drawFunctionList();
    
    danzeffRender(G2D_SCR_W/2 - DANZEFF_W/2, G2D_SCR_H - DANZEFF_H);
}

void UI::controls(Controls* ctrl)
{
    if (ctrl->buttonJustPressed(PSP_CTRL_START))
    {
        active = !active;
    }

    if (!active) return;
    
    selected += ctrl->buttonJustPressed(PSP_CTRL_DOWN) -
                ctrl->buttonJustPressed(PSP_CTRL_UP);
    if (selected < 0) selected = (int)manager->size() - 1;
    if (selected > (int)manager->size() - 1) selected = 0;
    
    edit(danzeffRead(*ctrl->getPad()));
}

bool UI::isActive()
{
    return active;
}
