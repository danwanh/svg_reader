#include "stdafx.h"
#include "Lib.h"
#include "Draw.h"
#include "ReadFile.h"

Figure gr;

float angle = 0.0f;
float scale = 1.0f;
float transX = 0.0f;
float transY = 0.0f;
VOID OnPaint(HDC hdc, Figure& gr, string filePath)

{
    Graphics graphics(hdc);
    Draw draw;
    //đọc file để chạy cmdline
    FileProcess file(filePath.c_str());
    vector<Shape*> s = file.ReadFile();
    gr.setList(s);
    draw.drawFigure(graphics, gr, angle, scale, transX, transY);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR cmdline, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    string svgFile = "svg-01.svg";
    if (cmdline != NULL && strlen(cmdline) > 0) svgFile = cmdline;

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("GettingStarted");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("GettingStarted"),   // window class name
        TEXT("SVG Demo"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        CW_USEDEFAULT,            // initial x size
        CW_USEDEFAULT,            // initial y size
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        (LPVOID)svgFile.c_str());      // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    static string svgFile;

    switch (message)
    {
    case WM_CREATE:
    {
        svgFile = reinterpret_cast<LPCSTR>(((LPCREATESTRUCT)lParam)->lpCreateParams);
        break;
    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc, gr, svgFile);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_MOUSEWHEEL:
    {
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta > 0) {
            scale *= 1.1f;
        }
        else {
            scale /= 1.1f;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            transX -= 50.0f;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RIGHT:
            transX += 50.0f;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_UP:
            transY -= 50.0f;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_DOWN:
            transY += 50.0f;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 'R':
            angle += 10.0f;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
// WndProc
