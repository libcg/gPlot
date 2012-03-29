#include "view.h"
#include <iostream>
#include <cmath>

View::View(FunctionManager* manager) :
    manager(manager),
    x(0.f), xs(0.f), xst(xs),
    y(0.f), ys(0.f), yst(ys),
            zs(1.f), zst(1.f),
    w(BASE_W), h(BASE_H)
{
    intraFontInit();
    sfont = intraFontLoad("flash0:/font/ltn8.pgf", INTRAFONT_CACHE_MED);

    manager->update(screenToViewX(0.f),
                    screenToViewX(G2D_SCR_W),
                    G2D_SCR_W);
}

View::~View()
{
    intraFontUnload(sfont);
}

FTYPE View::screenToViewX(float x)
{
    return ( x * this->w / G2D_SCR_W + this->x - this->w/2.f);
}

FTYPE View::screenToViewY(float y)
{
    return (-y * this->h / G2D_SCR_H + this->y + this->h/2.f);
}

float View::viewToScreenX(FTYPE x)
{
    return (( x - this->x + this->w/2.f) * G2D_SCR_W / this->w);
}

float View::viewToScreenY(FTYPE y)
{
    return (-(double)y + this->y + this->h/2.f) * G2D_SCR_H / this->h;
}

void View::camera()
{
    xs += (xst * MOVE_SPEED * w / BASE_W - xs) * MOVE_ACCEL;
    ys += (yst * MOVE_SPEED * h / BASE_H - ys) * MOVE_ACCEL;
    zs += (pow(2.f, -zst * ZOOM_SPEED) - zs) * ZOOM_ACCEL;
    
    x += xs;
    y += ys;
    w *= zs;
    h *= zs;
    
    if (fabs(xs) > TINY_SPEED || fabs(zs - 1.f) > TINY_ZOOM)
    {
        manager->update(screenToViewX(0.f),
                        screenToViewX(G2D_SCR_W),
                        G2D_SCR_W / 2);
    }
}

void View::drawOrigin()
{
    static float ox, oy;
    static float ivx, ivy;
    static int isx, isy;
    static float factor;
    
    ox = viewToScreenX(0.f);
    oy = viewToScreenY(0.f);
    
    g2dBeginLines((g2dLine_Mode)0);
    {
        g2dSetColor(LITEGRAY);
        
        if (ox >= 0.f && ox < G2D_SCR_W)
        {
            g2dSetCoordXY(ox, 0.f);
            g2dAdd();
            g2dSetCoordXY(ox, G2D_SCR_H);
            g2dAdd();
        }
        if (oy >= 0.f && oy < G2D_SCR_H)
        {
            g2dSetCoordXY(0.f, oy);
            g2dAdd();
            g2dSetCoordXY(G2D_SCR_W, oy);
            g2dAdd();
        }
    }
    g2dEnd();
    
    factor = pow(2.f, -floor(log(BASE_H / this->h) / log(2.f))) * FACTOR_F;
    ivx = floor(screenToViewX(0.f) / factor - 1.f) * factor;
    ivy = floor(screenToViewY(0.f) / factor + 1.f) * factor;
    
    intraFontSetStyle(sfont, FONT_SIZE, BLACK, 0, 0.f, 0);

    g2dBeginPoints();
    {
        g2dSetColor(DARKGRAY);

        sfont->options = INTRAFONT_ALIGN_CENTER;
        while ((isx = viewToScreenX(ivx += factor)) < G2D_SCR_W)
        {
            if (ivx == 0.f) continue;
            
            // Point
            isy = oy - 1.f;
            isy = (isy < 0.f ? 0.f : (isy >= G2D_SCR_H ? G2D_SCR_H-1 : isy));
            g2dSetCoordXY(isx, isy);
            g2dAdd();
            
            // Label
            if (isy < FONT_LIMIT)
            {
                isy += 10;
            }
            else
            {
                isy -= 4;
            }
            intraFontPrintf(sfont, isx, isy, "%.4g", ivx);
        }

        while ((isy = viewToScreenY(ivy -= factor)) < G2D_SCR_H)
        {
            if (ivy == 0.f) continue;
        
            // Point
            isx = ox + 1.f;
            isx = (isx < 0.f ? 0.f : (isx >= G2D_SCR_W ? G2D_SCR_W-1 : isx));
            g2dSetCoordXY(isx, isy);
            g2dAdd();
            
            // Label
            if (isx > G2D_SCR_W-FONT_LIMIT)
            {
                sfont->options = INTRAFONT_ALIGN_RIGHT;
                isx -= 3;
            }
            else
            {
                sfont->options = INTRAFONT_ALIGN_LEFT;
                isx += 4;
            }
            intraFontPrintf(sfont, isx, isy+3, "%.4g", ivy);
        }
    }
    g2dEnd();
}

void View::drawFunction()
{
    static FTYPE a, b;
    static float x, y;
    static Function *f;
    static std::vector<FTYPE> *values;

    for (unsigned int i=0; i<manager->size(); i++)
    {
        f = manager->getFunction(i);
        values = f->getValues();
        if (!f->isValid()) continue;
        
        g2dBeginLines(G2D_STRIP);
        {
            g2dSetColor(BLACK);

            for (unsigned int j=0; j<values->size(); j++)
            {      
                a = f->getA();
                b = f->getB();
                x = viewToScreenX(a + (b-a) * j / values->size());
                y = viewToScreenY(values->at(j));

                if (std::isnan(y)) continue;

                g2dSetCoordXY(x, y);
                g2dAdd();
            }
        }
        g2dEnd();
    }
}

void View::draw()
{
    camera();
    drawOrigin();
    drawFunction();
}

void View::controls(Controls* ctrl)
{
    xst = ctrl->buttonPressed(PSP_CTRL_RIGHT) -
          ctrl->buttonPressed(PSP_CTRL_LEFT);
    yst = ctrl->buttonPressed(PSP_CTRL_UP) -
          ctrl->buttonPressed(PSP_CTRL_DOWN);
    zst = ctrl->buttonPressed(PSP_CTRL_RTRIGGER) -
          ctrl->buttonPressed(PSP_CTRL_LTRIGGER);
          
    if (ctrl->buttonPressed(PSP_CTRL_CROSS))
    {
        manager->setFunction(0, "0.5*x^3");
    }
    if (ctrl->buttonPressed(PSP_CTRL_SQUARE))
    {
        manager->setFunction(1, "cos(x)");
    }
    if (ctrl->buttonPressed(PSP_CTRL_TRIANGLE))
    {
        manager->setFunction(2, "x*sin(x)");
    }
    if (ctrl->buttonPressed(PSP_CTRL_CIRCLE))
    {
        manager->setFunction(3, "exp(x)");
    }
}
