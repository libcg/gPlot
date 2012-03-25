#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include <muParser.h>
#include <muParserError.h>

class Function
{
private:
    mu::Parser parser;
    FTYPE cx, cy;
    bool valid;
    
public:
    Function();
    void set(std::string str);
    bool compute(FTYPE *y, FTYPE x);
    bool isValid();
};

#endif
