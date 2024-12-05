#include "stdafx.h"
#include "Circle.h"
<<<<<<< HEAD
//dananh
=======
//xuan
>>>>>>> 7eef9e9037413868b30c0135d6eeaa6087282987
circle::circle() {
    c.setX(0);
    c.setY(0);
    rx = 0;
    ry = 0;
};

circle::circle(float centerX, float centerY, float r, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t) : ellipse(centerX, centerY, r, r, stroke, strokeW, fill, "circle", t){ }


float circle::getRadius() {
    return this->rx;
}


void circle::setRadius(float radius) {
    this->rx = radius;
    this->ry = radius;
}

