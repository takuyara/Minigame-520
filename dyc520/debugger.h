//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
//It rained on us at 4am.
#pragma once
#define _CRE_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdarg>
extern FILE *fl;
#define debug(...) fprintf(fl, __VA_ARGS__), fflush(fl)
