#include "stdafx.h"
#include "Ellipse.h"
ellipse::ellipse() {
    c.setX(0);
    c.setY(0);
    rx = 0;
    ry = 0;
}

ellipse::ellipse(float centerX, float centerY, float radiusX, float radiusY, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t) :  Shape(stroke, strokeW, fill, name, t), rx(radiusX), ry(radiusY) {
    c.setX(centerX);
    c.setY(centerY);
    calculateBoundingBox();
}

float ellipse::getCx() {
    return this->c.getX();
}

float ellipse::getCy() {
    return this->c.getY();
}

float ellipse::getRx() {
    return this->rx;
}

float ellipse::getRy() {
    return this->ry;
}

void ellipse::setCx(float cx) {
    this->c.setX(cx);
}

void ellipse::setCy(float cy) {
    this->c.setY(cy);
}

void ellipse::setRx(float rx) {
    this->rx = rx;
}

void ellipse::setRy(float ry) {
    this->ry = ry;
}

void ellipse::calculateBoundingBox() {
    xmin = c.getX() - rx;
    ymin = c.getY() - ry;
    xmax = c.getX() + rx;
    ymax = c.getY() + ry;
}
