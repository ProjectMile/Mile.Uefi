﻿/** @file
  Native Platform Configuration Database (PCD) INFO PROTOCOL.

  The protocol that provides additional information about items that reside in the PCD database.

  Different with the EFI_GET_PCD_INFO_PROTOCOL defined in PI 1.2.1 specification,
  the native PCD INFO PROTOCOL provide interfaces for dynamic and dynamic-ex type PCD.
  The interfaces for dynamic type PCD do not require the token space guid as parameter,
  but interfaces for dynamic-ex type PCD require token space guid as parameter.

  Copyright (c) 2013 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Revision Reference:
  This Protocol was introduced in PI Specification 1.2.

**/

#ifndef __PCD_INFO_H__
#define __PCD_INFO_H__

extern EFI_GUID  gGetPcdInfoProtocolGuid;

#define GET_PCD_INFO_PROTOCOL_GUID \
  { 0x5be40f57, 0xfa68, 0x4610, { 0xbb, 0xbf, 0xe9, 0xc5, 0xfc, 0xda, 0xd3, 0x65 } }

///
/// The forward declaration for GET_PCD_INFO_PROTOCOL.
///
typedef struct _GET_PCD_INFO_PROTOCOL GET_PCD_INFO_PROTOCOL;

/**
  Retrieve additional information associated with a PCD token.

  This includes information such as the type of value the TokenNumber is associated with as well as possible
  human readable name that is associated with the token.

  @param[in]    TokenNumber The PCD token number.
  @param[out]   PcdInfo     The returned information associated with the requested TokenNumber.

  @retval  EFI_SUCCESS      The PCD information was returned successfully
  @retval  EFI_NOT_FOUND    The PCD service could not find the requested token number.
**/
typedef
EFI_STATUS
(EFIAPI *GET_PCD_INFO_PROTOCOL_GET_INFO)(
  IN        UINTN           TokenNumber,
  OUT       EFI_PCD_INFO    *PcdInfo
  );

/**
  Retrieve additional information associated with a PCD token.

  This includes information such as the type of value the TokenNumber is associated with as well as possible
  human readable name that is associated with the token.

  @param[in]    Guid        The 128-bit unique value that designates the namespace from which to extract the value.
  @param[in]    TokenNumber The PCD token number.
  @param[out]   PcdInfo     The returned information associated with the requested TokenNumber.

  @retval  EFI_SUCCESS      The PCD information was returned successfully
  @retval  EFI_NOT_FOUND    The PCD service could not find the requested token number.
**/
typedef
EFI_STATUS
(EFIAPI *GET_PCD_INFO_PROTOCOL_GET_INFO_EX)(
  IN CONST  EFI_GUID        *Guid,
  IN        UINTN           TokenNumber,
  OUT       EFI_PCD_INFO    *PcdInfo
  );

/**
  Retrieve the currently set SKU Id.

  @return   The currently set SKU Id. If the platform has not set at a SKU Id, then the
            default SKU Id value of 0 is returned. If the platform has set a SKU Id, then the currently set SKU
            Id is returned.
**/
typedef
UINTN
(EFIAPI *GET_PCD_INFO_PROTOCOL_GET_SKU)(
  VOID
  );

///
/// This is the PCD service to use when querying for some additional data that can be contained in the
/// PCD database.
///
struct _GET_PCD_INFO_PROTOCOL {
  ///
  /// Retrieve additional information associated with a PCD.
  ///
  GET_PCD_INFO_PROTOCOL_GET_INFO       GetInfo;
  GET_PCD_INFO_PROTOCOL_GET_INFO_EX    GetInfoEx;
  ///
  /// Retrieve the currently set SKU Id.
  ///
  GET_PCD_INFO_PROTOCOL_GET_SKU        GetSku;
};

#endif
