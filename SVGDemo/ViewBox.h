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

public:
    ViewBox();
    ViewBox(float x, float y, float w, float h);

    float getMinX() const;
    float getMinY() const;
    float getWidth() const;
    float getHeight() const;

    void setMinX(float x);
    void setMinY(float y);
    void setWidth(float w);
    void setHeight(float h);
};
#endif // !VIEWBOX_H
