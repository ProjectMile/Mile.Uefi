/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      limits.h
 * PURPOSE:   The C Standard Library <limits.h> header.
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#ifndef MILE_UEFI_LIMITS
#define MILE_UEFI_LIMITS

#include <Mile.Uefi.Specification.h>

#define CHAR_BIT    8
#define SCHAR_MIN   MIN_INT8
#define SCHAR_MAX   MAX_INT8
#define UCHAR_MAX   MAX_UINT8

#ifndef _CHAR_UNSIGNED
#define CHAR_MIN    SCHAR_MIN
#define CHAR_MAX    SCHAR_MAX
#else
#define CHAR_MIN    0
#define CHAR_MAX    UCHAR_MAX
#endif

#define MB_LEN_MAX  5
#define SHRT_MIN    MIN_INT16
#define SHRT_MAX    MAX_INT16
#define USHRT_MAX   MAX_UINT16
#define INT_MIN     MIN_INT32
#define INT_MAX     MAX_INT32
#define UINT_MAX    MAX_UINT32
#define LONG_MIN    MIN_INT32
#define LONG_MAX    MAX_INT32
#define ULONG_MAX   MAX_UINT32
#define LLONG_MAX   MAX_INT64
#define LLONG_MIN   MIN_INT64
#define ULLONG_MAX  MAX_UINT64

#endif // !MILE_UEFI_LIMITS
