#pragma once
#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "Shape.h"
class rectangle : public Shape {
private:
    point start;
    float width, height;

public:
    rectangle();
    rectangle(float, float, float, float, MyColor, float, MyColor, string, vector<TransformCommand>);

    float getRecX();
    float getRecY();
    float getWidth();
    float getHeight();

    void setRecX(float x);
    void setRecY(float y);
    void setWidth(float width);
    void setHeight(float height);

    void calculateBoundingBox() override;
};

#endif