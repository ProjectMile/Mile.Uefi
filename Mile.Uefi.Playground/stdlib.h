#ifndef STDLIB
#define STDLIB

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    static inline int abs(int __n)
    {
        return (__n < 0) ? -__n : __n;
    }

    static inline long labs(long __n)
    {
        return (__n < 0L) ? -__n : __n;
    }

    void* __cdecl calloc(
        size_t _Count,
        size_t _Size);

    void __cdecl free(
        void* _Block);

    void* __cdecl malloc(
        size_t _Size);

    int rand();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !STDLIB
