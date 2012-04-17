/* 
 * gPlot++ by Geecko
 *
 * This work is licensed under a Creative Commons license.
 * See LICENSE for more details.
 */

#ifndef FUNCTIONMANAGER_H
#define FUNCTIONMANAGER_H

#include "function.h"
#include "view.h"
#include <vector>
#include <pspkernel.h>

#define FUN_N 4

class View;

class FunctionManager
{
private:
    std::vector<Function*> functions;
    SceInt32 thid;
    bool dirty;
    FTYPE a, b;
    unsigned int n;

    static int thread(unsigned int args, void *argp);
    void setDirty(bool state);
    FTYPE getA();
    FTYPE getB();
    unsigned int getN();
    bool isDirty();

public:
    FunctionManager();
    ~FunctionManager();
    void update(FTYPE a, FTYPE b, unsigned int n);
    void setFunction(unsigned int i, std::string expr);
    Function* getFunction(unsigned int i);
    unsigned int size();
};

#endif
