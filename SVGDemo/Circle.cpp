#include "stdafx.h"
#include "Circle.h"
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

GraphicsPath* circle::createGraphicsPath() {
    GraphicsPath* path = new GraphicsPath();
    path->AddEllipse(c.getX() - rx, c.getY() - ry, 2 * rx, 2 * ry);
    return path;    
}


void circle::getBoundingBox(float& x, float& y, float& width, float& height) {
    x = c.getX() - rx;
    y = c.getY() - ry;
    width = 2 * rx;
    height = 2 * ry;
}
