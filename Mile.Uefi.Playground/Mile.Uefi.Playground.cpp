/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      Mile.Uefi.Playground.cpp
 * PURPOSE:   Implementation for Mile.Uefi UEFI Application Demo
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#ifndef _In_
#define _In_
#endif 

#ifndef _In_opt_
#define _In_opt_
#endif 

#ifndef _Out_
#define _Out_
#endif 

#ifndef _Out_opt_
#define _Out_opt_
#endif 

#include <Mile.Uefi.Core.h>

#include "op_stdint.h"

/*
 * Otokonoko Platform (OP) Status
 */
typedef uint32_t OPSTATUS;

#define OPSTATUS_SUCCESS ((OPSTATUS)0x00000000)

EFI_HANDLE g_ImageHandle;
EFI_SYSTEM_TABLE* g_SystemTable;
EFI_BOOT_SERVICES* g_BootServices;

EFI_GRAPHICS_OUTPUT_PROTOCOL* g_GraphicsOutputProtocol;

EFI_STATUS UefiOutputString(
    _In_ EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* Instance,
    const wchar_t* String)
{
    return Instance->OutputString(
        Instance,
        const_cast<CHAR16*>(reinterpret_cast<const CHAR16*>(String)));
}


[[noreturn]] void BugCheck()
{
    UefiOutputString(
        g_SystemTable->ConOut,
        L"\r\n"
        L"\r\n"
        L"BugCheck: Not implemented. [OPStatus = 0x000000BC]\r\n");


    for (;;) {}
}

/**
 * Copies the value ch (after conversion to unsigned char as if by
 * (unsigned char)ch) into each of the first count characters of the object
 * pointed to by dest. The behavior is undefined if access occurs beyond the
 * end of the dest array. The behavior is undefined if dest is a null pointer.
 *
 * @param dest The pointer to the object to fill.
 * @param ch The fill byte.
 * @param count The number of bytes to fill.
 * @return A copy of dest.
 */
extern "C" void* memset(void* dest, int ch, size_t count)
{
    // The "volatile" is used to prevent the compiler from trying to implement
    // these C functions as inline functions.
    volatile unsigned char* d = reinterpret_cast<unsigned char*>(dest);

    while (count--)
        *d++ = static_cast<unsigned char>(ch);

    return dest;
}

/**
 * Entry point to UEFI Application.
 *
 * @param ImageHandle The image handle of the UEFI Application.
 * @param SystemTable A pointer to the EFI System Table.
 * @return EFI_STATUS. If the function succeeds, the return value is
 *         EFI_SUCCESS.
 */
EFI_STATUS EFIAPI UefiMain(
    _In_ EFI_HANDLE        ImageHandle,
    _In_ EFI_SYSTEM_TABLE* SystemTable)
{
    g_ImageHandle = ImageHandle;
    g_SystemTable = SystemTable;
    g_BootServices = SystemTable->BootServices;

    // Disable UEFI watchdog timer
    g_BootServices->SetWatchdogTimer(0, 0, 0, nullptr);

    EFI_STATUS Status = EFI_SUCCESS;

    Status = g_BootServices->LocateProtocol(
        &gEfiGraphicsOutputProtocolGuid,
        nullptr,
        reinterpret_cast<void**>(&g_GraphicsOutputProtocol));
    if (!EFI_ERROR(Status))
    {
        uint32_t* FrameBufferBase = reinterpret_cast<uint32_t*>(
            g_GraphicsOutputProtocol->Mode->FrameBufferBase);
        size_t FrameBufferSize = static_cast<size_t>(
            g_GraphicsOutputProtocol->Mode->FrameBufferSize);

        for (size_t i = 0; i < FrameBufferSize; ++i)
        {
            // A R G B (BGRA)
            FrameBufferBase[i] = 0x00007ACC;
        }

    }
    else
    {
        UefiOutputString(
            g_SystemTable->ConOut,
            L"Failed to locate the Graphics Output Protocol.\r\n");
    }

    UefiOutputString(
        g_SystemTable->ConOut,
        L"Otokonoko no Sekai Version 0.1 [Build 1]\r\n"
        L"Copyright (C) 2019 Covariant Institute. All rights reserved.\r\n"
        L"\r\n");

    UefiOutputString(
        g_SystemTable->ConOut,
        L"Hello World\r\n");

    BugCheck();

    return EFI_SUCCESS;
}

#pragma comment(linker, "/entry:UefiMain")

