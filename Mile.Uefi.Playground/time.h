#ifndef TIME
#define TIME

#include <Mile.Uefi.Specification.h>

typedef INT64 time_t;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    time_t time(time_t* arg);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !TIME
