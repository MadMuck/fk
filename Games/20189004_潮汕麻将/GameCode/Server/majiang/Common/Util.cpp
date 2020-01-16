#include "Util.h"
#include <cstdlib>
#include <cstring>
extern "C"
{
#include <time.h>
}

void InitRandom()
{
	srand((unsigned int)time(NULL));
}

int Random()
{
    return rand();
}

bool ContainString(const char *str, int strLength, const char *pattern, int patternLength)
{
    bool ret = false;
    if(patternLength <= strLength)
    {
        ret = strncmp(str, pattern, patternLength) == 0;
    }
    return ret;
}

bool IsNumeric(char c)
{
    return c >= '0' && c <= '9';
}

int ToNumeric(char c)
{
    int ret = 0;
    if(IsNumeric(c)){ ret = c - '0'; }
    return ret;
}
