#include "function.h"

Function::Function() :
    parser(), valid(false)
{
    parser.DefineVar("x", &cx);
}

void Function::set(std::string str)
{
    parser.SetExpr(str);
    
    try
    {
        parser.Eval();
        valid = true;
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

bool Function::compute(float *y, float x)
{
    cx = x;
    
    try
    {
        cy = parser.Eval();
    }
    catch (mu::Parser::exception_type &error)
    {
        // An error happened...
        return true;
    }
    
    *y = cy;
    return false;
}

bool Function::isValid()
{
    return valid;
}
