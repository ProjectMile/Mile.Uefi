#ifndef STDDEF
#define STDDEF

#include <Mile.Uefi.Core.h>

typedef UINTN size_t;
typedef INTN ptrdiff_t;
#define offsetof(StrucName, Member)  OFFSET_OF(StrucName, Member)

#endif // !STDDEF

