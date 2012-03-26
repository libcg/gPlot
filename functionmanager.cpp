#include "functionmanager.h"

int FunctionManager::thread(unsigned int args, void *argp)
{
    FunctionManager* _this = *((FunctionManager**)argp);
    
    for (;;)
    {
        while (!_this->isDirty()) // SOON...
        {
            sceKernelDelayThread(0);
        }

        for (unsigned int i=0; i<_this->size(); i++)
        {
            Function* f = _this->getFunction(i);
            if (!f->isValid()) continue;
            
            f->computeRange(_this->getA(), _this->getB(), _this->getN());
        }

        _this->setDirty(false);
    }
    
    return 0;
}

FunctionManager::FunctionManager() :
    functions(5, (Function*)NULL), dirty(false)
{
    static FunctionManager *_this = this;

    for (unsigned int i=0; i<size(); i++)
    {
        functions[i] = new Function();
    }
    
    thid = sceKernelCreateThread("fmanager", thread, 0x18, 0x10000, 0, NULL);
    sceKernelStartThread(thid, sizeof(_this), &_this);
}

FunctionManager::~FunctionManager()
{
    sceKernelTerminateDeleteThread(thid);
    
    for (unsigned int i=0; i<size(); i++)
    {
        delete functions[i];
    }
}

void FunctionManager::update(FTYPE a, FTYPE b, unsigned int n)
{
    this->a = a;
    this->b = b;
    this->n = n;
    
    setDirty(true);
}

void FunctionManager::setFunction(unsigned int i, std::string expr)
{
    if (i >= size()) return;
    
    functions[i]->setExpr(expr);

    setDirty(true);
}

void FunctionManager::setDirty(bool state)
{
    dirty = state;
}

Function* FunctionManager::getFunction(unsigned int i)
{
    if (i >= size()) return NULL;
    
    return functions[i];
}

FTYPE FunctionManager::getA()
{
    return a;
}

FTYPE FunctionManager::getB()
{
    return b;
}

unsigned int FunctionManager::getN()
{
    return n;
}

unsigned int FunctionManager::size()
{
    return functions.size();
}

bool FunctionManager::isDirty()
{
    return dirty;
}
