#ifndef OP_STDDEF
#define OP_STDDEF

#include <Uefi.h>

typedef UINTN size_t;
typedef INTN ptrdiff_t;
#define offsetof(StrucName, Member)  OFFSET_OF(StrucName, Member)

#endif // !OP_STDDEF

