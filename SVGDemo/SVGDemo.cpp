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

    vector<pair<string, vector<point>>> pa = {

        // Sub-path starts
        {"M", {point(16.66, 11.25)}},                             // Move to
        {"L", {point(15.04, 12.27)}},                             // Line to
        {"L", {point(15.18, 14.67)}},                             // Line to
        {"C", {point(15.31, 16.01), point(14.85, 16.47), point(13.58, 16.62)}}, // Cubic Bézier curve
        {"L", {point(8.51, 17.22)}},                              // Line to
        {"C", {point(8.23176, 17.2579), point(8.13732, 16.9427), point(8.33589, 16.7441)}}, // Cubic Bézier curve
        {"L", {point(9.88, 15.2)}},                               // Line to
        {"C", {point(10.17, 14.91), point(10.17, 14.43), point(9.88, 14.14)}}, // Cubic Bézier curve
        {"L", {point(8.81, 14.14)}},                              // Line to
        {"L", {point(7.27478, 15.683)}},                              // Line to
        {"C", {point(7.07699, 15.8818), point(6.76211, 15.7879), point(6.8, 15.51)}}, // Cubic Bézier curve
        {"L", {point(7.39, 10.44)}},                              // Line to
        {"C", {point(7.55, 9.17), point(8.01, 8.76), point(9.35, 8.84)}}, // Cubic Bézier curve
        {"L", {point(11.74, 8.98)}},                              // Line to
        {"L", {point(12.77, 7.35)}},                              // Line to
        {"C", {point(13.24, 6.61), point(14.2, 6.57), point(14.9, 7.27)}}, // Cubic Bézier curve
        {"L", {point(16.75, 9.12)}},                              // Line to
        {"C", {point(17.41, 9.77), point(17.37, 10.8), point(16.66, 11.25)}}, // Cubic Bézier curve
        {"Z", {}}                                                 // Close path
    };



    Shape* p = new path();
    p->setFillColor(MyColor(41, 45, 50, 1));

    dynamic_cast<path*>(p)->setPath(pa);
    dynamic_cast<path*>(p)->calculateBoundingBox();

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

    string svgFile = "testcuathay.svg";
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
