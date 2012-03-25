#include "view.h"
#include <iostream>
#include <cmath>

View::View() :
    x(0.f), xs(0.f), xst(xs),
    y(0.f), ys(0.f), yst(ys),
            zs(1.f), zst(1.f),
    w(BASE_W), h(BASE_H),
    f()
{
    
    f.set("x < 0 ? x*x+1 : cos(x)");
}

float View::screenToViewX(FTYPE x)
{
    return ( x * this->w / G2D_SCR_W + this->x - this->w/2.f);
}

float View::screenToViewY(FTYPE y)
{
    return (-y * this->h / G2D_SCR_H + this->y + this->h/2.f);
}

FTYPE View::viewToScreenX(float x)
{
    return (( x - this->x + this->w/2.f) * G2D_SCR_W / this->w);
}

FTYPE View::viewToScreenY(float y)
{
    return ((-y + this->y + this->h/2.f) * G2D_SCR_H / this->h);
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
}

void View::drawOrigin()
{
    float ox, oy;
    int ivx, ivy;
    int isx, isy;
    
    ox = viewToScreenX(0.);
    oy = viewToScreenY(0.);
    
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
    
    ivx = (int)screenToViewX(0.f) - 1;
    ivy = (int)screenToViewY(0.f) + 1;
    
    g2dBeginPoints();
    {
        g2dSetColor(DARKGRAY);
        
        while ((isx = viewToScreenX(++ivx)) < G2D_SCR_W)
        {
            if (ivx == 0.f) continue;
            
            g2dSetCoordXY(isx, oy-1.f);
            g2dAdd();
        }
        while ((isy = viewToScreenY(--ivy)) < G2D_SCR_H)
        {
            if (ivy == 0.f) continue;
        
            g2dSetCoordXY(ox+1.f, isy);
            g2dAdd();
        }
    }
    g2dEnd();
}


void View::drawFunction()
{
    FTYPE y;
    
    if (f.isValid())
    {
        g2dBeginLines(G2D_STRIP);
        {
            g2dSetColor(BLACK);

            for (float i=0.f; i<G2D_SCR_W; i++)
            {      
                if (!f.compute(&y, screenToViewX(i)))   
                {
                    g2dSetCoordXY(i, viewToScreenY(y));
                    g2dAdd();
                }
            }
        }
        g2dEnd();
    }
    else g2dClear(RED);
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
}
