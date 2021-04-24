/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      stddef.h
 * PURPOSE:   The C Standard Library <stddef.h> header.
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#ifndef MILE_UEFI_STDDEF
#define MILE_UEFI_STDDEF

#include <Mile.Uefi.Specification.h>

typedef UINTN size_t;
typedef INTN ptrdiff_t;
#define offsetof(StrucName, Member) OFFSET_OF(StrucName, Member)

#endif // !MILE_UEFI_STDDEF

