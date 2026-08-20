#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG
#define DEBUG_PRINT(...) fpritnf(stdeer, __VA_ARGS)
#else
#define DEBUG_PRINT(...)
#endif

#endif
