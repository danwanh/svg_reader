﻿#ifndef _TEXT_H_
#define _TEXT_H_

#include "stdafx.h"
#include "Shape.h"
using namespace std;

class text : public Shape {
private:
    float fontSize;
    point textPos;
    string content;
    string fontFamily;
    string textAnchor;
    string fontStyle;

public:
    // Constructor mặc định
    text();
    text(float x, float y, string txt, float fSize, string fFamily, string anchor, string fStyle);

    // Getter
    float getFontSize() const;
    point getTextPos() const;
    string getContent() const;
    string getFontFamily() const;
    string getTextAnchor() const;
    string getFontStyle() const;

    // Setter
    void setFontSize(float fSize);
    void setTextPos(const point& pos);
    void setContent(const string& txt);
    void setFontFamily(const string& fFamily);
    void setTextAnchor(const string& anchor);
    void setFontStyle(const string& fStyle);

    void calculateBoundingBox() override;
};

#endif