#ifndef FUNCTION_H
#define FUNCTION_H

#include <muParser.h>
#include <muParserError.h>

class Function
{
private:
    mu::Parser parser;
    float cx, cy;
    bool valid;
    
public:
    Function();
    void set(std::string str);
    bool compute(float *y, float x);
    bool isValid();
};

#endif
