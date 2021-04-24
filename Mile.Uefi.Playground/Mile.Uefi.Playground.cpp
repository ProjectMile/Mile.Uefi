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

#include <Mile.Uefi.Specification.h>

#include <stddef.h>
#include <stdint.h>
#include "string.h"

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
//extern "C" void* memset(void* dest, int ch, size_t count)
//{
//    // The "volatile" is used to prevent the compiler from trying to implement
//    // these C functions as inline functions.
//    volatile unsigned char* d = reinterpret_cast<unsigned char*>(dest);
//
//    while (count--)
//        *d++ = static_cast<unsigned char>(ch);
//
//    return dest;
//}

/*
memcmp V
memcpy V
memset V
strcat V
strcmp V
strcpy V
strlen V
strncpy V
*/

extern "C" int memcmp(const void* s1, const void* s2, size_t n)
{
    volatile unsigned char* p1 = (unsigned char*)s1;
    volatile unsigned char* p2 = (unsigned char*)s2;

    while (n-- > 0)
    {
        if (*p1 < *p2)
        {
            return -1;
        }
        else if (*p1 > *p2)
        {
            return 1;
        }

        p1++;
        p2++;
    }
    return 0;
}

extern "C" void* memcpy(void* dest, const void* src, size_t n)
{
    volatile unsigned char* pout = (unsigned char*)dest;
    volatile unsigned char* pin = (unsigned char*)src;
    while (n-- > 0) *pout++ = *pin++;
    return dest;
}

extern "C" void* memset(void* s, int c, size_t n)
{
    volatile unsigned char* p = (unsigned char*)s;
    while (n-- > 0) *p++ = static_cast<unsigned char>(c);
    return s;
}

extern "C" char* strcat(char* dest, const char* src)
{
    char* ret = dest;

    dest += strlen(dest);
    while (*src != '\0')
    {
        *dest++ = *src++;
    }
    *dest = '\0';

    return ret;
}

extern "C" int strcmp(const char* cs, const char* ct)
{
    volatile signed char result;
    for (; ; )
    {
        if ((result = *cs - *ct++) != 0 || !*cs++)
            break;
    }

    return result;
}

extern "C" char* strcpy(char* dest, const char* src)
{
    char* tmp = dest;
    while ((*dest++ = *src++) != '\0');
    return tmp;
}

extern "C" size_t strlen(const char* s)
{
    volatile const char* sc;
    for (sc = s; *sc != '\0'; ++sc);
    return sc - s;
}

extern "C" char* strncpy(char* dest, const char* src, size_t n)
{
    char* ret = dest;     /* Value to be returned */
    char* end = dest + n; /* End of dest buffer + 1 byte */

    /* Copy up n bytes, breaking out of the loop early if a NUL terminator is
     * encountered.
     */

    while ((dest != end) && (*dest++ = *src++) != '\0')
    {
    }

    /* Note that there may be no NUL terminator in 'dest' */

    /* Pad the remainder of the array pointer to 'dest' with NULs */

    while (dest != end)
    {
        *dest++ = '\0';
    }

    return ret;
}



#include <lvgl/lvgl.h>
#include "lv_demo_widgets.h"

lv_disp_drv_t g_LvglDisplayDriver;
lv_disp_buf_t g_LvglDisplayBuffer;
lv_color_t g_LvglDisplayRawBuffer[1048576];


void LvglDisplayDriverFlushCallback(
    lv_disp_drv_t* disp_drv,
    const lv_area_t* area,
    lv_color_t* color_p)
{
    area;

    g_GraphicsOutputProtocol->Blt(
        g_GraphicsOutputProtocol,
        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*)color_p,
        EfiBltBufferToVideo,
        0,
        0,
        area->x1,
        area->y1,
        area->x2 - area->x1 + 1,
        area->y2 - area->y1 + 1,
        0);

    ::lv_disp_flush_ready(disp_drv);
}

void LvglDisplayDriverRounderCallback(
    lv_disp_drv_t* disp_drv,
    lv_area_t* area)
{
    area->x1 = 0;
    area->x2 = disp_drv->hor_res - 1;
    area->y1 = 0;
    area->y2 = disp_drv->ver_res - 1;
}

void EFIAPI Timeout(
    IN EFI_EVENT      Event,
    IN VOID* Context)
{
    ::lv_tick_inc(10);
    ::lv_task_handler();
    /*::UefiOutputString(
        g_SystemTable->ConOut,
        L"Tick.\r\n");*/
    return;
}

EFI_SERIAL_IO_PROTOCOL* g_SerialIoProtocol = nullptr;

void serial_send(const char* text)
{
    UINTN length = strlen(text);
    g_SerialIoProtocol->Write(g_SerialIoProtocol, &length, const_cast<void*>(reinterpret_cast<const void*>(text)));
}

void my_log_cb(lv_log_level_t level, const char* file, uint32_t line, const char* fn_name, const char* dsc)
{
    /*Send the logs via serial port*/
    if (level == LV_LOG_LEVEL_ERROR) serial_send("ERROR: ");
    if (level == LV_LOG_LEVEL_WARN)  serial_send("WARNING: ");
    if (level == LV_LOG_LEVEL_INFO)  serial_send("INFO: ");
    if (level == LV_LOG_LEVEL_TRACE) serial_send("TRACE: ");

    serial_send("File: ");
    serial_send(file);

    //char line_str[8];
    //sprintf(line_str, "%d", line);
    //serial_send("#");
    //serial_send(line_str);

    //serial_send(": ");
    serial_send(fn_name);
    serial_send(": ");
    serial_send(dsc);
    serial_send("\n");
}

EFI_STATUS LvglUefiHalInit()
{
    EFI_STATUS Status = EFI_SUCCESS;

    EFI_HANDLE handles[128];
    UINTN handles_size = sizeof(handles);
    size_t handles_count = 0;

    Status = g_BootServices->LocateHandle(
        EFI_LOCATE_SEARCH_TYPE::ByProtocol,
        &gEfiSerialIoProtocolGuid,
        nullptr,
        &handles_size,
        &handles[0]);
    handles_count = handles_size / sizeof(EFI_HANDLE);
    if (Status == EFI_SUCCESS && handles_count != 0)
    {
        g_BootServices->HandleProtocol(
            handles[0],
            &gEfiSerialIoProtocolGuid,
            reinterpret_cast<void**>(&g_SerialIoProtocol));
        if (EFI_ERROR(Status))
        {
            ::UefiOutputString(
                g_SystemTable->ConOut,
                L"Failed to open the Serial I/O Protocol.\r\n");

            return Status;
        }
    }
    else
    {
        ::UefiOutputString(
            g_SystemTable->ConOut,
            L"Failed to locate the Serial I/O Protocol handle.\r\n");

        return Status;
    }

    Status = g_BootServices->LocateProtocol(
        &gEfiGraphicsOutputProtocolGuid,
        nullptr,
        reinterpret_cast<void**>(&g_GraphicsOutputProtocol));
    if (EFI_ERROR(Status))
    {
        ::UefiOutputString(
            g_SystemTable->ConOut,
            L"Failed to locate the Graphics Output Protocol.\r\n");

        return Status;
    }

    EFI_HANDLE TimerOne = nullptr;

    Status = g_BootServices->CreateEvent(EVT_NOTIFY_SIGNAL | EVT_TIMER,
        TPL_CALLBACK,
        Timeout,
        nullptr,
        &TimerOne);
    if (EFI_ERROR(Status))
    {
        ::UefiOutputString(
            g_SystemTable->ConOut,
            L"Failed to create the event.\r\n");

        return Status;
    }

    Status = g_BootServices->SetTimer(
        TimerOne,
        TimerPeriodic,
        10 * 10000);
    if (EFI_ERROR(Status))
    {
        ::UefiOutputString(
            g_SystemTable->ConOut,
            L"Failed to set the timer.\r\n");

        return Status;
    }

    ::lv_disp_drv_init(&g_LvglDisplayDriver);

    ::lv_disp_buf_init(
        &g_LvglDisplayBuffer,
        g_LvglDisplayRawBuffer,
        nullptr,
        1048576);

    g_LvglDisplayDriver.hor_res = static_cast<lv_coord_t>(
        g_GraphicsOutputProtocol->Mode->Info->HorizontalResolution);
    g_LvglDisplayDriver.ver_res = static_cast<lv_coord_t>(
        g_GraphicsOutputProtocol->Mode->Info->VerticalResolution);
    g_LvglDisplayDriver.flush_cb = ::LvglDisplayDriverFlushCallback;
    g_LvglDisplayDriver.buffer = &g_LvglDisplayBuffer;
    g_LvglDisplayDriver.rounder_cb = ::LvglDisplayDriverRounderCallback;

    ::lv_disp_drv_register(&g_LvglDisplayDriver);

    ::lv_log_register_print_cb(my_log_cb);

    return EFI_SUCCESS;
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

    ::lv_init();

    if (EFI_ERROR(::LvglUefiHalInit()))
    {
        ::BugCheck();
    }

    lv_demo_widgets();

    for (;;)
    {
        g_BootServices->Stall(10 * 1000);
    }

    return EFI_SUCCESS;
}

#pragma comment(linker, "/entry:UefiMain")

