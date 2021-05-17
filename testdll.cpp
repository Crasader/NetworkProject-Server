#include "testdll.hpp"

extern "C" float addInC(float a, float b) { return a + b; }

extern "C" TestLibrary *instance_create() { return new TestLibrary(); }
extern "C" float add(TestLibrary *tl, float a, float b) { return tl->add(a, b); }
extern "C" float substract(TestLibrary *tl, float a, float b) { return tl->substract(a, b); }

float TestLibrary::add(float a, float b)
{
    return a + b;
}

float TestLibrary::substract(float a, float b)
{
    return a - b;
}