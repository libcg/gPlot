#ifndef MAIN_H
#define MAIN_H

#include "functionmanager.h"
#include "controls.h"
#include "view.h"
#include "ui.h"

class Main
{
private:
    Controls *ctrl;
    FunctionManager *manager;
    View *view;

public:
    Main();
    ~Main();
    int run();
};

#endif
