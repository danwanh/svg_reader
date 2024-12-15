#include "stdafx.h"
#include "Text.h"
text::text()
    : fontSize(16), textPos({ 0.0f, 0.0f }),
    content(""), dx(0), dy(0), fontFamily("Times New Roman"), textAnchor("start"), fontStyle("normal") {}

text::text(float x, float y, float dx_, float dy_, string txt, float fSize, string fFamily, string anchor, string fStyle)
    : fontSize(fSize), textPos(point(x, y)), content(txt),
    fontFamily(fFamily), textAnchor(anchor), fontStyle(fStyle), dx(dx_), dy(dy_) {
}
float text::getFontSize() const { 
    return fontSize; 
}
point text::getTextPos() const { 
    return textPos; 
}
string text::getContent() const { 
    return content;
}
string text::getFontFamily() const { 
    return fontFamily; 
}
string text::getTextAnchor() const { 
    return textAnchor; 
}
string text::getFontStyle() const { 
    return fontStyle; 
}

void text::setFontSize(float fSize) { 
    fontSize = fSize;
}
void text::setTextPos(const point& pos) {
    textPos = pos; 
}
void text::setContent(const string& txt) { 
    content = txt; 
}
void text::setFontFamily(const string& fFamily) {
    fontFamily = fFamily; 
}
void text::setTextAnchor(const string& anchor) { 
    textAnchor = anchor; 
}
void text::setFontStyle(const string& fStyle) { 
    fontStyle = fStyle; 
}
void text::setDx(float dx) {
    this->dx = dx;
}

void text::setDy(float dy) {
    this->dy = dy;
}

//test thử nhiều cái đi, nếu text ko chạy đc thì ktra ở 2 hàm này, nó có thể sai ở chỗ (1)!!!!
GraphicsPath* text::createGraphicsPath()
{
    GraphicsPath* path = new GraphicsPath();

    // Chuyển fontFamily từ string sang WCHAR (GDI+ yêu cầu WCHAR)
    wstring wFontFamily = fontFamily.empty() ? L"Times New Roman" : wstring(fontFamily.begin(), fontFamily.end()); //(1)
    wstring wContentTemp(content.begin(), content.end());   //chuyển content sang WCHAR
    const WCHAR* wContent = wContentTemp.c_str();

    // Chọn FontStyle
    int gdiFontStyle = FontStyleRegular; // Mặc định
    if (fontStyle == "bold") gdiFontStyle = FontStyleBold;
    else if (fontStyle == "italic") gdiFontStyle = FontStyleItalic;
    else if (fontStyle == "bolditalic") gdiFontStyle = FontStyleBoldItalic;

    // Tạo FontFamily và Font
    FontFamily family(wFontFamily.c_str());
    Font font(&family, fontSize, gdiFontStyle, UnitPixel);

    // Xác định vị trí text
    HDC hdc = GetDC(nullptr);
    Graphics graphics(hdc);
    PointF position(textPos.getX() + dx, textPos.getY() + dy);

    // Đo bounding box để điều chỉnh text-anchor
    RectF boundingBox;
    graphics.MeasureString(wContent, -1, &font, position, &boundingBox);

    if (textAnchor == "middle") {
        position.X -= boundingBox.Width / 2;
    }
    else if (textAnchor == "end") {
        position.X -= boundingBox.Width;
    }

    // Tạo GraphicsPath cho văn bản
    path->AddString(wContent, -1, &family, gdiFontStyle, fontSize, position, nullptr);

    ReleaseDC(nullptr, hdc);
    return path;
}

void text::getBoundingBox(float& x, float& y, float& width, float& height)
{
    // Khởi tạo GDI+ Graphics để đo văn bản
    HDC hdc = GetDC(nullptr);
    Graphics graphics(hdc);

    // Chuyển fontFamily từ string sang WCHAR
    wstring wFontFamily = fontFamily.empty() ? L"Times New Roman" : wstring(fontFamily.begin(), fontFamily.end()); //(1)
    wstring wContentTemp(content.begin(), content.end());
    const WCHAR* wContent = wContentTemp.c_str();

    // Chọn FontStyle
    int gdiFontStyle = FontStyleRegular;
    if (fontStyle == "bold") gdiFontStyle = FontStyleBold;
    else if (fontStyle == "italic") gdiFontStyle = FontStyleItalic;
    else if (fontStyle == "bolditalic") gdiFontStyle = FontStyleBoldItalic;

    // Tạo Font và FontFamily
    FontFamily family(wFontFamily.c_str());
    Font font(&family, fontSize, gdiFontStyle, UnitPixel);

    // Đo kích thước văn bản
    PointF position(textPos.getX() + dx, textPos.getY() + dy);
    RectF boundingBox;
    graphics.MeasureString(wContent, -1, &font, position, &boundingBox);

    // Điều chỉnh text-anchor cho x
    if (textAnchor == "middle") {
        x = position.X - boundingBox.Width / 2;
    }
    else if (textAnchor == "end") {
        x = position.X - boundingBox.Width;
    }
    else { //default
        x = boundingBox.X;
    }

    y = boundingBox.Y;
    width = boundingBox.Width;
    height = boundingBox.Height;

    // Giải phóng DC
    ReleaseDC(nullptr, hdc);
}

float text::getDx() {
    return this->dx;
}

float text::getDy() {
    return this->dy;
}

