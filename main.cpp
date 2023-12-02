#include <iostream>

#ifdef __win32
#include <windows.h>
#endif

/* For Testing Only. */

int main(int args, char *argv[])
{
#ifndef __win32
    return 0;
#endif

    return 0;
}