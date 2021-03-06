/* 
 * gPlot++ by Geecko
 *
 * This work is licensed under a Creative Commons license.
 * See LICENSE for more details.
 */

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
    MUP_BASETYPE cx, cy;
    bool valid, init, access;
    std::vector<FTYPE>* values;
    FTYPE a, b;
    
public:
    Function();
    ~Function();
    void setExpr(std::string str);
    void setAccess(bool state);
    bool compute(FTYPE *y, FTYPE x);
    void computeRange(FTYPE a, FTYPE b, unsigned int n);
    bool isValid();
    std::string getExpr();
    std::vector<FTYPE>* getValues();
    FTYPE getA();
    FTYPE getB();
};

#endif
