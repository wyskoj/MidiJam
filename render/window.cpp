//
// Created by Jacob on 3/17/2026.
//


#include "window.h"

#include "gl.h"
#include "text.h"

extern HINSTANCE g_hInstance;
extern bool g_fullscreen;
extern HWND g_hWnd;
extern HDC g_hdc;
extern int g_pixelFormatInitialized;
extern PIXELFORMATDESCRIPTOR g_pixelFormatDescriptor;
extern HGLRC g_openglRenderingContext;
extern char g_keyStateArray[];
extern char g_lastKeyPressed;
extern char g_isWindowActive;
extern char g_leftButtonDown;
extern char g_middleButtonDown;
extern char g_rightButtonDown;
extern char g_leftButtonClicked;
extern char g_middleButtonClicked;
extern char g_rightButtonClicked;
extern char g_mouseWheelMoved;
extern char g_mouseMoved;
extern short g_mouseX;
extern short g_mouseY;
extern short g_mouseWheelDelta;

// FUNCTION: MIDIJAM 0x446450
LRESULT CALLBACK HandleWindowBehavior(const HWND hWnd, const UINT Msg, const WPARAM wParam, const LPARAM lParam) {
    if (Msg > WM_LBUTTONDOWN) {
        switch (Msg) {
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
                g_mouseWheelMoved = 1;
                g_mouseWheelDelta += static_cast<short>(HIWORD(wParam));
                return 0;
            default:
                return DefWindowProcA(hWnd, Msg, wParam, lParam);
        }
    }
    else if (Msg == WM_LBUTTONDOWN) {
        if (!g_leftButtonDown)
            g_leftButtonClicked = 1;
        g_leftButtonDown = 1;
        return 0;
    }
    else {
        if (Msg > WM_KEYDOWN) {
            switch (Msg) {
                case WM_KEYUP:
                    g_keyStateArray[wParam] = 0;
                    return 0;
                case WM_SYSCOMMAND:
                    if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
                        return 0;
                    break;
                case WM_MOUSEMOVE:
                    g_mouseMoved = 1;
                    g_mouseY = static_cast<short>(HIWORD(lParam));
                    g_mouseX = static_cast<short>(LOWORD(lParam));
                    return 0;
            }
        }
        else {
            switch (Msg) {
                case WM_KEYDOWN:
                    g_keyStateArray[wParam] = 1;
                    g_lastKeyPressed = static_cast<char>(wParam);
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
bool __cdecl CreateMidijamWindow(const LPCSTR lpWindowName, const HINSTANCE hInstance, const GLsizei windowWidth,
                                 const GLsizei windowHeight, const int bitDepth, const DWORD refreshRate,
                                 const bool isFullscreen, FILE* hwfStream, char* hwfAppendixPtr,
                                 const unsigned int hwfAppendixEntriesCount) {
    DEVMODEA DevMode; // [esp+0h] [ebp-E8h] BYREF
    WNDCLASSA WndClass; // [esp+A4h] [ebp-44h] BYREF
    DWORD dwExStyle; // [esp+CCh] [ebp-1Ch]
    DWORD dwStyle; // [esp+D0h] [ebp-18h]
    RECT windowRect; // [esp+D4h] [ebp-14h] BYREF
    int pixelFormat; // [esp+E4h] [ebp-4h]

    windowRect.left = 0;
    windowRect.right = windowWidth;
    windowRect.top = 0;
    windowRect.bottom = windowHeight;
    g_hInstance = hInstance;
    g_fullscreen = isFullscreen;
    g_hInstance = GetModuleHandleA(0);
    WndClass.style = 35; // CS_HREDRAW | CS_VREDRAW | CS_OWNDC
    WndClass.lpfnWndProc = HandleWindowBehavior;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = g_hInstance;
    WndClass.hIcon = LoadIconA(0, IDI_WINLOGO);
    WndClass.hCursor = LoadCursorA(0, IDC_ARROW);
    WndClass.hbrBackground = 0;
    WndClass.lpszMenuName = 0;
    WndClass.lpszClassName = "OpenGL";
    if (RegisterClassA(&WndClass)) {
        if (!g_fullscreen)
            goto LABEL_8;
        memset(&DevMode, 0, sizeof(DevMode));
        DevMode.dmSize = sizeof(DEVMODEA);
        DevMode.dmPelsWidth = windowWidth;
        DevMode.dmPelsHeight = windowHeight;
        DevMode.dmBitsPerPel = bitDepth;
        if (refreshRate)
            DevMode.dmDisplayFrequency = refreshRate;
        DevMode.dmFields = 0x5C0000; // AI says DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY
        // Set fullscreen
        if (ChangeDisplaySettingsA(&DevMode, CDS_FULLSCREEN)) {
            PostQuitMessage(0);
            return false;
        }
        else {
        LABEL_8:
            if (g_fullscreen) {
                dwExStyle = WS_EX_APPWINDOW;
                dwStyle = WS_POPUP;
            }
            else {
                dwExStyle = 0x40100; // AI says: WS_EX_APPWINDOW | WS_EX_WINDOWEDGE
                dwStyle = WS_OVERLAPPEDWINDOW;
            }
            ShowCursor(FALSE); // hide cursor
            AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
            g_hWnd = CreateWindowExA(
                dwExStyle,
                "OpenGL",
                lpWindowName,
                dwStyle | 0x6000000, // AI says: dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
                0,
                0,
                windowRect.right - windowRect.left,
                windowRect.bottom - windowRect.top,
                0,
                0,
                g_hInstance,
                0);
            if (!g_hWnd)
                goto CLEANUP_AND_FAIL;
            if ((g_pixelFormatInitialized & 1) == 0) {
                g_pixelFormatInitialized |= 1u;
                g_pixelFormatDescriptor.cColorBits = bitDepth;
                g_pixelFormatDescriptor.cRedBits = 0;
                g_pixelFormatDescriptor.cRedShift = 0;
                g_pixelFormatDescriptor.cGreenBits = 0;
                g_pixelFormatDescriptor.cGreenShift = 0;
                g_pixelFormatDescriptor.cBlueBits = 0;
                g_pixelFormatDescriptor.cBlueShift = 0;
                g_pixelFormatDescriptor.cAlphaBits = 0;
                g_pixelFormatDescriptor.cAlphaShift = 0;
                g_pixelFormatDescriptor.cAccumBits = 0;
                g_pixelFormatDescriptor.cAccumRedBits = 0;
                g_pixelFormatDescriptor.cAccumGreenBits = 0;
                g_pixelFormatDescriptor.cAccumBlueBits = 0;
                g_pixelFormatDescriptor.cAccumAlphaBits = 0;
                g_pixelFormatDescriptor.cDepthBits = 16;
                g_pixelFormatDescriptor.cStencilBits = 1;
                g_pixelFormatDescriptor.cAuxBuffers = 0;
                g_pixelFormatDescriptor.iLayerType = 0;
                g_pixelFormatDescriptor.bReserved = 0;
                g_pixelFormatDescriptor.dwLayerMask = 0;
                g_pixelFormatDescriptor.dwVisibleMask = 0;
                g_pixelFormatDescriptor.dwDamageMask = 0;
            }
            g_hdc = GetDC(g_hWnd);
            if (!g_hdc)
                goto CLEANUP_AND_FAIL;
            pixelFormat = ChoosePixelFormat(g_hdc, &g_pixelFormatDescriptor);
            if (pixelFormat
                && SetPixelFormat(g_hdc, pixelFormat, &g_pixelFormatDescriptor)
                && (g_openglRenderingContext = wglCreateContext(g_hdc)) != 0
                && wglMakeCurrent(g_hdc, g_openglRenderingContext)
                && (ShowWindow(g_hWnd, 5),
                    SetForegroundWindow(g_hWnd),
                    SetFocus(g_hWnd),
                    SetPerspectiveProjection(windowWidth, windowHeight),
                    LoadHwfFonts(hwfStream, hwfAppendixPtr, hwfAppendixEntriesCount))
                && (CreateFontDisplayLists(), InitLightsCullingAndInputs())) {
                return TRUE;
            }
            else {
            CLEANUP_AND_FAIL:
                MidiJamWindowCleanup();
                return FALSE;
            }
        }
    }
    else {
        PostQuitMessage(0);
        return FALSE;
    }
}

// FUNCTION: MIDIJAM 0x4471F0
// MATCH: EXACT
void MidiJamWindowCleanup() {
    if (g_fullscreen) {
        ChangeDisplaySettingsA(NULL, 0);
        ShowCursor(TRUE);
    }
    if (g_openglRenderingContext) {
        if (!wglMakeCurrent(NULL, NULL))
            MessageBoxA(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        if (!wglDeleteContext(g_openglRenderingContext))
            MessageBoxA(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_openglRenderingContext = NULL;
    }
    if (g_hdc && !ReleaseDC(g_hWnd, g_hdc)) {
        MessageBoxA(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hdc = NULL;
    }
    if (g_hWnd && !DestroyWindow(g_hWnd)) {
        MessageBoxA(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hWnd = NULL;
    }
    if (!UnregisterClassA("OpenGL", g_hInstance)) {
        MessageBoxA(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_ICONASTERISK);
        g_hInstance = NULL;
    }
    DeleteFontDisplayLists();
    PostQuitMessage(0);
}
