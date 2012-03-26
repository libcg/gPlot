#ifndef VIEW_H
#define VIEW_H

#include "common.h"
#include "controls.h"
#include "functionmanager.h"
#include <glib2d.h>

#define BASE_W 30.f
#define BASE_H 17.f
#define MOVE_ACCEL 0.06f
#define MOVE_SPEED 0.4f
#define ZOOM_ACCEL 0.06f
#define ZOOM_SPEED 0.045f
#define TINY_SPEED 0.001f
#define TINY_ZOOM  0.001f

class FunctionManager;

class View
{
private:
    FunctionManager* manager;
    float x, xs, xst;
    float y, ys, yst;
    float    zs, zst;
    float w, h;

    float screenToViewX(FTYPE x);
    float screenToViewY(FTYPE y);
    FTYPE viewToScreenX(float x);
    FTYPE viewToScreenY(float y);
    void camera();
    void drawOrigin();
    void drawFunction();

public:
    View(FunctionManager* manager);
    void draw();
    void controls(Controls* ctrl);
};

#endif
