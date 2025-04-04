﻿/** @file
  Real Time clock Architectural Protocol as defined in PI Specification VOLUME 2 DXE

  This code abstracts time and data functions. Used to provide
  Time and date related EFI runtime services.

  The GetTime (), SetTime (), GetWakeupTime (), and SetWakeupTime () UEFI 2.0
  services are added to the EFI system table and the
  EFI_REAL_TIME_CLOCK_ARCH_PROTOCOL_GUID protocol is registered with a NULL
  pointer.

  No CRC of the EFI system table is required, since that is done in the DXE core.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __ARCH_PROTOCOL_REAL_TIME_CLOCK_H__
#define __ARCH_PROTOCOL_REAL_TIME_CLOCK_H__

///
/// Global ID for the Real Time Clock Architectural Protocol
///
#define EFI_REAL_TIME_CLOCK_ARCH_PROTOCOL_GUID \
  { 0x27CFAC87, 0x46CC, 0x11d4, {0x9A, 0x38, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D } }

extern EFI_GUID  gEfiRealTimeClockArchProtocolGuid;

#endif
