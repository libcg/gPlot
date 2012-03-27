#include "function.h"
#include <pspthreadman.h>

Function::Function() :
    valid(false)
{
    parser = new mu::Parser();
    parser->DefineVar("x", &cx);
    
    values = new std::vector<FTYPE>();
}

Function::~Function()
{
    delete parser;
    delete values;
}

void Function::setExpr(std::string str)
{
    parser->SetExpr(str);
    valid = true;
    
    try
    {
        parser->Eval();
    }
    catch (mu::Parser::exception_type &error)
    {
        switch (error.GetCode())
        {
            case mu::ecDOMAIN_ERROR:
            case mu::ecDIV_BY_ZERO:
            case mu::ecCOUNT:
            break;
            
            default:
                // Expression not valid, can't be computed...
                valid = false;
            break;
        }
    }
}

bool Function::compute(FTYPE *y, FTYPE x)
{
    if (!valid) return true;

    cx = x;
    
    try
    {
        cy = parser->Eval();
    }
    catch (mu::Parser::exception_type &error)
    {
        // An error happened...
        return true;
    }
    
    *y = cy;
    return false;
}

void Function::computeRange(FTYPE a, FTYPE b, unsigned int n)
{
    std::vector<FTYPE> *tvalues = new std::vector<FTYPE>(n, 0.), *dvalues;

    for (unsigned int i=0; i<tvalues->size(); i++)
    {
        compute(&tvalues->at(i), a + (b-a) / n * i);
    }
    
    dvalues = values;
    values = tvalues;
    delete dvalues;
}

bool Function::isValid()
{
    return valid;
}

std::string Function::getExpr()
{
    return parser->GetExpr();
}

std::vector<FTYPE>* Function::getValues()
{
    return values;
}
