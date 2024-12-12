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
float text::getDx() {
    return this->dx;
}

float text::getDy() {
    return this->dy;
}

