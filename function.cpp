#include "function.h"
#include <pspthreadman.h>

Function::Function() :
    valid(false), init(true), access(false)
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
    init = false;

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
    
    init = true;
}

void Function::setAccess(bool state)
{
    access = state;
}

bool Function::compute(FTYPE *y, FTYPE x)
{
    if (!valid) return true;

    // Function::compute cause crashs if the object is not initialized. 
    while (!init) sceKernelDelayThread(1000);

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
        compute(&tvalues->at(i), a + (b-a) * i / n);
        
        sceKernelDelayThread(1000);
    }
    
    // Wait for no access to change values, because values size can change too.
    while (access) sceKernelDelayThread(1000);
    
    dvalues = this->values;
    this->values = tvalues;
    this->a = a;
    this->b = b;
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

FTYPE Function::getA()
{
    return a;
}

FTYPE Function::getB()
{
    return b;
}
