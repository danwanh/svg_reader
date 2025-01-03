#include "stdafx.h" 
#include "Lib.h"
#include "Draw.h"
#include "FileProcess.h"
#include "ViewBox.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR cmdline, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    string svgFile = "23127284_23127318_23127488_23127361_1.svg";
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
    static Figure figure;
    static float transX = 0.0f, transY = 0.0f; // Dịch chuyển ngang và dọc
    static float scale = 1.0f; // Zoom
    static float angle = 0.0f;
    static string svgFile;

    switch (message)
    {
    case WM_CREATE:
    {
        svgFile = reinterpret_cast<LPCSTR>(((LPCREATESTRUCT)lParam)->lpCreateParams); //command line
        FileProcess file(svgFile.c_str());
        vector<Shape*> s = file.ReadFile();
        figure.setList(s);
        figure.setViewBox(file.GetViewBox());
        break;
    }
    case WM_PAINT: {
    Draw:
        hdc = BeginPaint(hWnd, &ps);
        Graphics graphics(hdc);
        Draw draw;
        ViewBox* vb = figure.getViewBox();

        RECT window;
        GetClientRect(hWnd, &window);
        float portWidth = vb->getPortWidth();
        float portHeight = vb->getPortHeight();
        if (portWidth == 0) {
            portWidth = static_cast<float>(window.right - window.left); // kich thuoc cua so
            //vb->setPortWidth(portWidth);
        }
        if (portHeight == 0) {
            portHeight = static_cast<float>(window.bottom - window.top);
            //vb->setPortWidth(portHeight);
        }

        // Ensure fallback defaults if window size is invalid
        if (portWidth <= 0) portWidth = 200.0f;   // Default width
        if (portHeight <= 0) portHeight = 200.0f;

        float minX = vb->getMinX();
        float minY = vb->getMinY();
        float viewWidth = vb->getViewWidth();
        float viewHeight = vb->getViewHeight();

        float scaleX = 1.0f, scaleY = 1.0f, scaleXY = 1.0f;
        float offsetX = 0.0f, offsetY = 0.0f;

        // Tính toán scale và offset
        if (viewWidth > 0 && viewHeight > 0)
        {
            scaleX = portWidth / viewWidth;
            scaleY = portHeight / viewHeight;
            scaleXY = (scaleX < scaleY) ? scaleX : scaleY;

            offsetX = (portWidth - viewWidth * scaleXY) / 2.0f;
            offsetY = (portHeight - viewHeight * scaleXY) / 2.0f;
        }

        // Set clipping cho viewBox
        RectF clipRect(0, 0, portWidth, portHeight);
        graphics.SetClip(clipRect, CombineModeIntersect);
        // Áp dụng Transformations
        graphics.TranslateTransform(offsetX + transX, offsetY + transY);
        graphics.ScaleTransform(scale * scaleXY, scale * scaleXY);
        graphics.TranslateTransform(-minX, -minY);

        draw.drawFigure(graphics, figure, angle, scale, transX, transY, vb);
        EndPaint(hWnd, &ps);
        return 0;
    }
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
        case 'L': case 'l':
            angle += 10.0f;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 'R': case 'r':
            angle -= 10.0f;
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
