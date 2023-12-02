#ifndef WINDOWS_FILE_API_COMMON_H
#define WINDOWS_FILE_API_COMMON_H
#include <iostream>
#include <string.h>
#include <stdlib.h>

#if __cplusplus < 202002
#define WRONG_VERSION
#endif

#ifdef __WIN32
#include <windows.h>

#ifdef assert
#undef assert
#endif

#define assert(cond, err_msg, ...)              \
    if(!(cond))                                 \
    {                                           \
        fprintf(stderr, err_msg, ##__VA_ARGS__);\
        exit(EXIT_FAILURE);                     \
    }

#define error(err_msg, ...)                     \
{                                               \
    fprintf(stderr, err_msg, ##__VA_ARGS__);    \
    exit(EXIT_FAILURE);                         \
}
#endif

#endif