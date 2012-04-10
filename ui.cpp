#include "ui.h"
#include <string>

UI::UI(FunctionManager* manager, intraFont *sfont) :
    manager(manager), sfont(sfont),
    active(false), activated(false),
    selected(0), cursor(0)
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
    str = str.substr(0, str.size()-1); // Delete the ending space

    switch (c)
    {
        case '\010': // Del
            if (cursor > 0)
            {
                str = str.substr(0, cursor-1) +
                      str.substr(cursor, str.length());
                cursor--;
            }
            else if (str.length() > 0)
            {
                str = str.substr(1, str.length());
            }
            break;
            
        case DANZEFF_LEFT: // Move
            if (cursor > 0) cursor--;
            break;
        case DANZEFF_RIGHT:
            if (cursor < (int)str.length()) cursor++;
            break;

        case DANZEFF_SELECT: // Clear
            str.clear();
            cursor = 0;
            break;

        case DANZEFF_START:  // Do nothing
        case '\0':
            return;
            
        default: // Type
            str = str.substr(0, cursor) +
                  (char)c +
                  str.substr(cursor, str.length());
            cursor++;
            break;                
    }
    
    manager->setFunction(selected, str);
}

void UI::drawFunctionList()
{
    static int blink = 0;
    if (++blink >= 60) blink = 0;

    for (unsigned int i=0; i<manager->size(); i++)
    {
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

            // Color
            g2dPush();
            g2dSetColor(F_COLOR(i));
            g2dAdd();
            g2dSetCoordXYRelative(10, 0);
            g2dSetAlpha(0);
            g2dAdd();
            g2dSetCoordXYRelative(0, ELEMENT_H);
            g2dAdd();
            g2dSetCoordXYRelative(-10, 0);
            g2dSetAlpha(255);
            g2dAdd();
            g2dPop();
        }
        g2dEnd();
    }

    intraFontActivate(sfont);
   
    for (unsigned int i=0; i<manager->size(); i++)
    {        
        // Expression and cursor
        Function* f = manager->getFunction(i);
        std::string str = f->getExpr();
        float x;
        
        intraFontSetStyle(sfont, 0.9f, (f->isValid() ? BLACK : RED), 0, 0.f,
                          INTRAFONT_ALIGN_LEFT | INTRAFONT_ACTIVE);
        intraFontPrint(sfont, 10, i * ELEMENT_H + 13, str.c_str());
        
        if ((int)i == selected && blink / 30 == 0) // 0.5s blink
        {
            x = intraFontMeasureTextEx(sfont, str.c_str(), cursor);
            sfont->color = BLACK;
            sfont->size = 1.f;
            intraFontPrint(sfont, 10 + x, i * ELEMENT_H + 13 - 0.5f, "|");
        }
    }
    
    // Shadow
    g2dBeginQuads(NULL);
    {
        g2dSetColor(BLACK);
        g2dSetAlpha(255/4);
        g2dSetCoordXY(0, manager->size() * ELEMENT_H);
        g2dAdd();
        g2dSetCoordXYRelative(G2D_SCR_W, 0);
        g2dAdd();
        g2dSetCoordXYRelative(0, 2*ELEMENT_H);
        g2dSetAlpha(0);
        g2dAdd();
        g2dSetCoordXYRelative(-G2D_SCR_W, 0);
        g2dAdd();
    }
    g2dEnd();
}

void UI::draw()
{
    if (!activated)
    {
        intraFontSetStyle(sfont, 1.f, BLACK, WHITE, 0.f, INTRAFONT_ALIGN_RIGHT);                          
        intraFontPrint(sfont, G2D_SCR_W-10, G2D_SCR_H-10, "Press start...");
    }

    if (!active) return;
    
    drawFunctionList();

    danzeffRender(G2D_SCR_W/2 - DANZEFF_W/2, G2D_SCR_H - DANZEFF_H);
}

void UI::controls(Controls* ctrl)
{
    if (ctrl->buttonJustPressed(PSP_CTRL_START))
    {
        active = !active;
        activated = true;
    }

    if (!active) return;
    
    // Selector
    selected += ctrl->buttonJustPressed(PSP_CTRL_DOWN) -
                ctrl->buttonJustPressed(PSP_CTRL_UP);
    if (selected < 0) selected++;
    if (selected > (int)manager->size() - 1) selected--;

    std::string str = manager->getFunction(selected)->getExpr();
    str = str.substr(0, str.size()-1); // Delete the ending space
    if (cursor > (int)str.length()) cursor = str.length();
    
    // Edit
    edit(danzeffRead(*ctrl->getPad()));
}

bool UI::isActive()
{
    return active;
}
