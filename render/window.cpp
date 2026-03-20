//
// Created by Jacob on 3/17/2026.
//

#include "window.h"

#include "gl.h"
#include "text.h"

extern HINSTANCE           g_hInstance;
extern bool                g_fullscreen;
extern HWND                g_hWnd;
extern HDC                 g_hdc;
extern int                 g_pixelFormatInitialized;
extern PIXELFORMATDESCRIPTOR g_pixelFormatDescriptor;
extern HGLRC               g_openglRenderingContext;
extern char                g_keyStateArray[];
extern char                g_lastKeyPressed;
extern char                g_isWindowActive;
extern char                g_leftButtonDown;
extern char                g_middleButtonDown;
extern char                g_rightButtonDown;
extern char                g_leftButtonClicked;
extern char                g_middleButtonClicked;
extern char                g_rightButtonClicked;
extern char                g_mouseWheelMoved;
extern char                g_mouseMoved;
extern short               g_mouseX;
extern short               g_mouseY;
extern short               g_mouseWheelDelta;

// FUNCTION: MIDIJAM 0x446450
LRESULT CALLBACK HandleWindowBehavior(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (Msg > WM_LBUTTONDOWN)
    {
        switch (Msg)
        {
            case WM_LBUTTONUP:
                g_leftButtonDown = 0;
                return 0;
            case WM_RBUTTONDOWN:
                if (!g_rightButtonDown)
                    g_rightButtonClicked = 1;
                g_rightButtonDown = 1;
                return 0;
            case WM_RBUTTONUP:
                g_rightButtonDown = 0;
                return 0;
            case WM_MBUTTONDOWN:
                if (!g_middleButtonDown)
                    g_middleButtonClicked = 1;
                g_middleButtonDown = 1;
                return 0;
            case WM_MBUTTONUP:
                g_middleButtonDown = 0;
                return 0;
            case WM_MOUSEWHEEL:
                g_mouseWheelMoved  = 1;
                g_mouseWheelDelta += static_cast<short>(HIWORD(wParam));
                return 0;
            default:
                return DefWindowProcA(hWnd, Msg, wParam, lParam);
        }
    }
    else if (Msg == WM_LBUTTONDOWN)
    {
        if (!g_leftButtonDown)
            g_leftButtonClicked = 1;
        g_leftButtonDown = 1;
        return 0;
    }
    else
    {
        if (Msg > WM_KEYDOWN)
        {
            switch (Msg)
            {
                case WM_KEYUP:
                    g_keyStateArray[wParam] = 0;
                    return 0;
                case WM_SYSCOMMAND:
                    if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
                        return 0;
                    break;
                case WM_MOUSEMOVE:
                    g_mouseMoved = 1;
                    g_mouseY     = static_cast<short>(HIWORD(lParam));
                    g_mouseX     = static_cast<short>(LOWORD(lParam));
                    return 0;
            }
        }
        else
        {
            switch (Msg)
            {
                case WM_KEYDOWN:
                    g_keyStateArray[wParam] = 1;
                    g_lastKeyPressed        = static_cast<char>(wParam);
                    return 0;
                case WM_SIZE:
                    return 0;
                case WM_ACTIVATE:
                    g_isWindowActive = (HIWORD(wParam) == 0);
                    return 0;
                case WM_CLOSE:
                    PostQuitMessage(0);
                    return 0;
            }
        }
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }
}

// FUNCTION: MIDIJAM 0x446690
bool CreateMidijamWindow(
    LPCSTR       lpWindowName,
    HINSTANCE    hInstance,
    int          windowWidth,
    int          windowHeight,
    int          bitDepth,
    DWORD        refreshRate,
    bool         isFullscreen,
    FILE*        hwfStream,
    char*        hwfAppendixPtr,
    unsigned int hwfAppendixEntriesCount)
{
    RECT windowRect = { 0, 0, windowWidth, windowHeight };

    g_hInstance  = hInstance;
    g_fullscreen = isFullscreen;
    g_hInstance  = GetModuleHandleA(nullptr);

    WNDCLASSA wndClass        = {};
    wndClass.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.lpfnWndProc      = HandleWindowBehavior;
    wndClass.cbClsExtra       = 0;
    wndClass.cbWndExtra       = 0;
    wndClass.hInstance        = g_hInstance;
    wndClass.hIcon            = LoadIconA(nullptr, IDI_WINLOGO);
    wndClass.hCursor          = LoadCursorA(nullptr, IDC_ARROW);
    wndClass.hbrBackground    = nullptr;
    wndClass.lpszMenuName     = nullptr;
    wndClass.lpszClassName    = "OpenGL";

    if (!RegisterClassA(&wndClass))
    {
        PostQuitMessage(0);
        return false;
    }

    if (g_fullscreen)
    {
        DEVMODEA devMode        = {};
        devMode.dmSize          = sizeof(DEVMODEA);
        devMode.dmPelsWidth     = windowWidth;
        devMode.dmPelsHeight    = windowHeight;
        devMode.dmBitsPerPel    = bitDepth;
        if (refreshRate)
            devMode.dmDisplayFrequency = refreshRate;
        devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

        if (ChangeDisplaySettingsA(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            PostQuitMessage(0);
            return false;
        }
    }

    DWORD dwExStyle;
    DWORD dwStyle;
    if (g_fullscreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_POPUP;
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle   = WS_OVERLAPPEDWINDOW;
    }

    ShowCursor(FALSE);
    AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

    g_hWnd = CreateWindowExA(
        dwExStyle,
        "OpenGL",
        lpWindowName,
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        0, 0,
        windowRect.right  - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, nullptr, g_hInstance, nullptr);

    if (!g_hWnd)
    {
        MidiJamWindowCleanup();
        return false;
    }

    if (!(g_pixelFormatInitialized & 1))
    {
        g_pixelFormatInitialized            |= 1;
        g_pixelFormatDescriptor.cColorBits   = static_cast<BYTE>(bitDepth);
        g_pixelFormatDescriptor.cRedBits     = 0;
        g_pixelFormatDescriptor.cRedShift    = 0;
        g_pixelFormatDescriptor.cGreenBits   = 0;
        g_pixelFormatDescriptor.cGreenShift  = 0;
        g_pixelFormatDescriptor.cBlueBits    = 0;
        g_pixelFormatDescriptor.cBlueShift   = 0;
        g_pixelFormatDescriptor.cAlphaBits   = 0;
        g_pixelFormatDescriptor.cAlphaShift  = 0;
        g_pixelFormatDescriptor.cAccumBits   = 0;
        g_pixelFormatDescriptor.cAccumRedBits   = 0;
        g_pixelFormatDescriptor.cAccumGreenBits = 0;
        g_pixelFormatDescriptor.cAccumBlueBits  = 0;
        g_pixelFormatDescriptor.cAccumAlphaBits = 0;
        g_pixelFormatDescriptor.cDepthBits   = 16;
        g_pixelFormatDescriptor.cStencilBits = 1;
        g_pixelFormatDescriptor.cAuxBuffers  = 0;
        g_pixelFormatDescriptor.iLayerType   = 0;
        g_pixelFormatDescriptor.bReserved    = 0;
        g_pixelFormatDescriptor.dwLayerMask  = 0;
        g_pixelFormatDescriptor.dwVisibleMask = 0;
        g_pixelFormatDescriptor.dwDamageMask  = 0;
    }

    g_hdc = GetDC(g_hWnd);
    if (!g_hdc)
    {
        MidiJamWindowCleanup();
        return false;
    }

    const int pixelFormat = ChoosePixelFormat(g_hdc, &g_pixelFormatDescriptor);
    if (!pixelFormat
        || !SetPixelFormat(g_hdc, pixelFormat, &g_pixelFormatDescriptor)
        || !((g_openglRenderingContext = wglCreateContext(g_hdc)))
        || !wglMakeCurrent(g_hdc, g_openglRenderingContext))
    {
        MidiJamWindowCleanup();
        return false;
    }

    ShowWindow(g_hWnd, SW_SHOW);
    SetForegroundWindow(g_hWnd);
    SetFocus(g_hWnd);
    SetPerspectiveProjection(windowWidth, windowHeight);

    if (!LoadHwfFonts(hwfStream, hwfAppendixPtr, hwfAppendixEntriesCount))
    {
        MidiJamWindowCleanup();
        return false;
    }

    CreateFontDisplayLists();

    if (!InitLightsCullingAndInputs())
    {
        MidiJamWindowCleanup();
        return false;
    }

    return true;
}

// FUNCTION: MIDIJAM 0x4471F0
void MidiJamWindowCleanup()
{
    if (g_fullscreen)
    {
        ChangeDisplaySettingsA(nullptr, 0);
        ShowCursor(TRUE);
    }

    if (g_openglRenderingContext)
    {
        if (!wglMakeCurrent(nullptr, nullptr))
            MessageBoxA(nullptr, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        if (!wglDeleteContext(g_openglRenderingContext))
            MessageBoxA(nullptr, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_openglRenderingContext = nullptr;
    }

    if (g_hdc && !ReleaseDC(g_hWnd, g_hdc))
    {
        MessageBoxA(nullptr, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hdc = nullptr;
    }

    if (g_hWnd && !DestroyWindow(g_hWnd))
    {
        MessageBoxA(nullptr, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hWnd = nullptr;
    }

    if (!UnregisterClassA("OpenGL", g_hInstance))
    {
        MessageBoxA(nullptr, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hInstance = nullptr;
    }

    DeleteFontDisplayLists();
    PostQuitMessage(0);
}