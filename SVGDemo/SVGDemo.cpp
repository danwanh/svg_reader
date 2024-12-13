#include "stdafx.h"
#include "Lib.h"
#include "Draw.h"
#include "FileProcess.h"

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
   /* FileProcess file(filePath.c_str());
    vector<Shape*> s = file.ReadFile();*/
    linearGradient* grad = new linearGradient();

    // Danh sách các điểm dừng màu
    vector<stop> stops = {
        {MyColor(255, 255, 221, 1), 0.0},   // Màu #fd5 tại offset 0
        {MyColor(255, 84, 84, 1), 0.5},    // Màu #ff543e tại offset 0.5
        {MyColor(255, 0, 0, 1), 1.0}    // Màu #c837ab tại offset 1
    };
    grad->setColorStop(stops);
    vector<TransformCommand> transGrad = { TransformCommand("translate", -0.7, -0.7, 30, 0.5, 0.5) };
    grad->setTransform(transGrad);

    // Đặt tọa độ gradient
    grad->setX1(0.0);
    grad->setY1(0.0);
    grad->setX2(0.0);
    grad->setY2(1.0);
    grad->setType(GradientType::LINEAR);


    //radialGradient* grad = new radialGradient;

    //// Danh sách các điểm dừng màu
    //vector<stop> stops = {
    //    {MyColor(255, 255, 221, 1), 0.0},   // vàng
    //    {MyColor(255, 84, 84, 1), 0.5},   // 
    //   {MyColor(255, 0, 0, 1), 1.0}    // đỏ
    //};
    //grad->setColorStop(stops);

    //// Đặt tọa độ gradient
    //grad->setCx(0.5);
    //grad->setCy(0.5);
    //grad->setFx(0.5);
    //grad->setFy(0.5);
    //grad->setR(0.5);
    ////grad->setfR(0);
    //grad->setType(GradientType::RADIAL);
    //vector<TransformCommand> transGrad = { TransformCommand("scale", 0.3, 0.4, 30, 0.5, 0.5) };
    //grad->setTransform(transGrad);
    // 
    // 
 
    ////Tạo hình chữ nhật
    vector<TransformCommand> trans = { TransformCommand("translate", 100, 100, 0, 1, 1) };
    rectangle* rect = new rectangle(0, 0, 400, 400, MyColor(255, 0, 0, 1), 1, MyColor(255, 255, 0, 1), "rect", trans);
    
    rect->setGradient(grad);
    //Vẽ
    draw.drawRectangle(graphics, rect);

    //gr.setList(s);
    //draw.drawFigure(graphics, gr, angle, scale, transX, transY);
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