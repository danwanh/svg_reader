#pragma once
#ifndef _LINE_H_
#define _LINE_H_

#include "Shape.h"

using namespace std;

class line : public Shape {
private:
    point start;
    point end;

public:
    line();
    line(float startX, float startY, float endX, float endY, MyColor stroke, float strokeW, string name, vector<TransformCommand> t);

    float getX1();
    float getX2();
    float getY1();
    float getY2();

    void setX1(float x1);
    void setX2(float x2);
    void setY1(float y1);
    void setY2(float y2);

    void calculateBoundingBox() override;
    
};
#endif