#ifndef MAIN_H
#define MAIN_H

#include "lib/controls.h"
#include "view.h"

class Main
{
private:
    Controls ctrl;
    View view;

public:
    Main();
    ~Main();
    int run();
};

#endif
