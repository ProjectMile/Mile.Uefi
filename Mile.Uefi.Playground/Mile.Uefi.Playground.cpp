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
#include <lv_demos/lv_demo.h>

lv_disp_drv_t g_LvglDisplayDriver;
lv_disp_draw_buf_t g_LvglDisplayBuffer;
lv_color_t g_LvglDisplayRawBuffer[1048576];

lv_indev_drv_t g_LvglPointerDriver;
lv_indev_t* g_LvglPointerDevice;

lv_indev_drv_t g_LvglKeyboardDriver;
lv_indev_t* g_LvglKeyboardDevice;

static lv_group_t* volatile g_DefaultGroup = nullptr;

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

void EFIAPI Timeout(
    IN EFI_EVENT      Event,
    IN VOID* Context)
{
    ::lv_tick_inc(10);
    return;
}

EFI_SERIAL_IO_PROTOCOL* g_SerialIoProtocol = nullptr;

void my_log_cb(
    const char* buf)
{
    char buffer[32768];

    UINTN length = ::lv_snprintf(
        buffer,
        32768,
        "%s\n",
        buf);

    g_SerialIoProtocol->Write(
        g_SerialIoProtocol,
        &length,
        buffer);
}

EFI_STATUS MileUefiConnectAllControllers()
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINTN HandlesCount = 0;
    EFI_HANDLE* Handles = nullptr;

    Status = g_BootServices->LocateHandleBuffer(
        EFI_LOCATE_SEARCH_TYPE::ByProtocol,
        &gEfiDevicePathProtocolGuid,
        nullptr,
        &HandlesCount,
        &Handles);
    if (!EFI_ERROR(Status))
    {
        for (UINTN i = 0; i < HandlesCount; ++i)
        {
            g_BootServices->ConnectController(
                Handles[i],
                nullptr,
                nullptr,
                TRUE);
        }

        g_BootServices->FreePool(Handles);
    }

    return Status;
}

EFI_SIMPLE_POINTER_PROTOCOL* g_SimplePointerProtocol;

void win_drv_read(
    lv_indev_drv_t* indev_drv,
    lv_indev_data_t* data)
{
    indev_drv;

    EFI_SIMPLE_POINTER_STATE State;
    if (!EFI_ERROR(g_SimplePointerProtocol->GetState(
        g_SimplePointerProtocol,
        &State)))
    {
        data->state = static_cast<lv_indev_state_t>(
            State.LeftButton ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL);
        data->point.x += State.RelativeMovementX / g_SimplePointerProtocol->Mode->ResolutionX * 4;
        data->point.y += State.RelativeMovementY / g_SimplePointerProtocol->Mode->ResolutionY * 4;

        if (data->point.x < 0)
        {
            data->point.x = 0;
        }
        if (data->point.x > g_LvglDisplayDriver.hor_res)
        {
            data->point.x = g_LvglDisplayDriver.hor_res;
        }

        if (data->point.y < 0)
        {
            data->point.y = 0;
        }
        if (data->point.y > g_LvglDisplayDriver.ver_res)
        {
            data->point.y = g_LvglDisplayDriver.ver_res;
        }
    }
}

extern "C" const lv_img_dsc_t mouse_cursor_icon;

EFI_SIMPLE_TEXT_INPUT_PROTOCOL* g_SimpleTextInputProtocol;

void win_kb_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data)
{
    (void)indev_drv;      /*Unused*/

    EFI_INPUT_KEY InputKey;
    if (!EFI_ERROR(g_SimpleTextInputProtocol->ReadKeyStroke(
        g_SimpleTextInputProtocol,
        &InputKey)))
    {
        switch (InputKey.ScanCode)
        {
        case SCAN_UP:
            data->key = LV_KEY_UP;
            break;
        case SCAN_DOWN:
            data->key = LV_KEY_DOWN;
            break;
        case SCAN_LEFT:
            data->key = LV_KEY_LEFT;
            break;
        case SCAN_RIGHT:
            data->key = LV_KEY_RIGHT;
            break;
        case SCAN_ESC:
            data->key = LV_KEY_ESC;
            break;
        case SCAN_DELETE:
            data->key = LV_KEY_DEL;
            break;
        /*case VK_BACK:
            data->key = LV_KEY_BACKSPACE;
            break;
        case VK_RETURN:
            data->key = LV_KEY_ENTER;
            break;*/
        case SCAN_PAGE_DOWN:
            data->key = LV_KEY_NEXT;
            break;
        case SCAN_PAGE_UP:
            data->key = LV_KEY_PREV;
            break;
        case SCAN_HOME:
            data->key = LV_KEY_HOME;
            break;
        case SCAN_END:
            data->key = LV_KEY_END;
            break;
        default:
            if (InputKey.UnicodeChar == CHAR_BACKSPACE)
            {
                data->key = LV_KEY_BACKSPACE;
            }
            else if (InputKey.UnicodeChar == CHAR_CARRIAGE_RETURN)
            {
                data->key = LV_KEY_ENTER;
            }
            else
            {
                data->key = InputKey.UnicodeChar;
            }
            
            break;
        }

        data->state = LV_INDEV_STATE_PR;
    }
}

EFI_STATUS LvglUefiHalInit()
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = ::MileUefiConnectAllControllers();
    if (EFI_ERROR(Status))
    {
        ::UefiOutputString(
            g_SystemTable->ConOut,
            L"Failed to connect all controllers.\r\n");

        return Status;
    }

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

    ::lv_log_register_print_cb(my_log_cb);

    EFI_HANDLE TimerOne = nullptr;

    Status = g_BootServices->CreateEvent(
        EVT_NOTIFY_SIGNAL | EVT_TIMER,
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

    ::lv_disp_drv_init(&g_LvglDisplayDriver);

    ::lv_disp_draw_buf_init(
        &g_LvglDisplayBuffer,
        g_LvglDisplayRawBuffer,
        nullptr,
        1048576);

    g_LvglDisplayDriver.hor_res = static_cast<lv_coord_t>(
        g_GraphicsOutputProtocol->Mode->Info->HorizontalResolution);
    g_LvglDisplayDriver.ver_res = static_cast<lv_coord_t>(
        g_GraphicsOutputProtocol->Mode->Info->VerticalResolution);
    g_LvglDisplayDriver.flush_cb = ::LvglDisplayDriverFlushCallback;
    g_LvglDisplayDriver.draw_buf = &g_LvglDisplayBuffer;

    ::lv_disp_drv_register(&g_LvglDisplayDriver);

    g_DefaultGroup = ::lv_group_create();
    ::lv_group_set_default(g_DefaultGroup);

    Status = g_BootServices->LocateProtocol(
        &gEfiSimplePointerProtocolGuid,
        nullptr,
        reinterpret_cast<void**>(&g_SimplePointerProtocol));
    if (EFI_ERROR(Status))
    {
        ::UefiOutputString(
            g_SystemTable->ConOut,
            L"Failed to locate the Simple Pointer Protocol.\r\n");

        return Status;
    }

    ::lv_indev_drv_init(&g_LvglPointerDriver);
    g_LvglPointerDriver.type = LV_INDEV_TYPE_POINTER;
    g_LvglPointerDriver.read_cb = ::win_drv_read;
    g_LvglPointerDevice = ::lv_indev_drv_register(&g_LvglPointerDriver);
    ::lv_indev_set_group(g_LvglPointerDevice, g_DefaultGroup);

    lv_obj_t* cursor_obj = lv_img_create(lv_scr_act());
    ::lv_img_set_src(cursor_obj, &mouse_cursor_icon);
    ::lv_indev_set_cursor(g_LvglPointerDevice, cursor_obj);
    
    Status = g_BootServices->LocateProtocol(
        &gEfiSimpleTextInProtocolGuid,
        nullptr,
        reinterpret_cast<void**>(&g_SimpleTextInputProtocol));
    if (EFI_ERROR(Status))
    {
        ::UefiOutputString(
            g_SystemTable->ConOut,
            L"Failed to locate the Simple Text Input Protocol.\r\n");

        return Status;
    }

    ::lv_indev_drv_init(&g_LvglKeyboardDriver);
    g_LvglKeyboardDriver.type = LV_INDEV_TYPE_KEYPAD;
    g_LvglKeyboardDriver.read_cb = ::win_kb_read;
    g_LvglKeyboardDevice = ::lv_indev_drv_register(&g_LvglKeyboardDriver);
    ::lv_indev_set_group(g_LvglKeyboardDevice, g_DefaultGroup);

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

    ::lv_demo_widgets();
    //::lv_demo_benchmark();
    //::lv_demo_keypad_encoder();

    for (;;)
    {
        ::lv_timer_handler();
        g_BootServices->Stall(10 * 1000);
    }

    return EFI_SUCCESS;
}

#pragma comment(linker, "/entry:UefiMain")

