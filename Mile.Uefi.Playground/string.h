#ifndef STRING
#define STRING

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    int memcmp(
        void const* _Buf1,
        void const* _Buf2,
        size_t _Size);

    void* memcpy(
        void* _Dst,
        void const* _Src,
        size_t _Size);

    void* __cdecl memset(
        void* _Dst,
        int _Val,
        size_t _Size);

    char* strcat(
        char* _Destination,
        char const* _Source);

    int strcmp(
        char const* _Str1,
        char const* _Str2);

    char* strcpy(
        char* _Destination,
        char const* _Source);

    size_t __cdecl strlen(
        char const* _Str);

    char* strncpy(
        char* _Destination,
        char const* _Source,
        size_t _Count);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !STRING
