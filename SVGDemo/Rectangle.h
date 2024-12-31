#pragma once
#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "Shape.h"
class rectangle : public Shape {
private:
    point start;
    float width, height;
    float rx, ry;

public:
    rectangle();
    rectangle(float, float, float, float, MyColor, float, MyColor, string, vector<TransformCommand>);
    rectangle(float, float,float, float, float, float, MyColor, float, MyColor, string, vector<TransformCommand>);

    float getRecX();
    float getRecY();
    float getRx();
    float getRy();
    float getWidth();
    float getHeight();

    void setRx(float x);
    void setRy(float y);
    void setRecX(float x);
    void setRecY(float y);
    void setWidth(float width);
    void setHeight(float height);

    GraphicsPath* createGraphicsPath() override;
    void getBoundingBox(float& x, float& y, float& width, float& height) override;
};

#endif