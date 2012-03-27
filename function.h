#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include <string>
#include <muParser.h>
#include <muParserError.h>

class Function
{
private:
    mu::Parser *parser;
    FTYPE cx, cy;
    bool valid;
    std::vector<FTYPE>* values;
    
public:
    Function();
    ~Function();
    void setExpr(std::string str);
    bool compute(FTYPE *y, FTYPE x);
    void computeRange(FTYPE a, FTYPE b, unsigned int n);
    bool isValid();
    std::string getExpr();
    std::vector<FTYPE>* getValues();
};

#endif
