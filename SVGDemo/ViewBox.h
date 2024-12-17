#pragma once
#ifndef VIEWBOX_H
#define VIEWBOX_H

#include "Lib.h"
using namespace std;

class ViewBox {
private:
    float minX;
    float minY;
    float width;
    float height;
    float portHeight, portWidth;

public:
    ViewBox();
    ViewBox(float x, float y, float w, float h, float pH, float pW)
        : minX(x), minY(y), width(w), height(h), portHeight(pH), portWidth(pW) {}
    float getPortHeight() const;
    float getPortWidth() const;
    float getMinX() const;
    float getMinY() const;
    float getViewWidth() const;
    float getViewHeight() const;

    void setPortHeight(float x);
    void setPortWidth(float y);
    void setMinX(float x);
    void setMinY(float y);
    void setViewWidth(float w);
    void setViewHeight(float h);

};
#endif // !VIEWBOX_H
