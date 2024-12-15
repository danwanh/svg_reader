#include "stdafx.h"
#include "Line.h"

line::line(){
    start.setX(0);
    start.setY(0);
    end.setX(0);
    end.setY(0);
}

line::line(float startX, float startY, float endX, float endY, MyColor stroke, float strokeW, string name, vector<TransformCommand> t) : Shape(stroke, strokeW, MyColor(0, 0, 0, 0), name, t) {
    start.setX(startX);
    start.setY(startY);
    end.setX(endX);
    end.setY(endY);
}

float line::getX1() {
    return this->start.getX();
}

float line::getX2() {
    return this->end.getX();
}

float line::getY1() {
    return this->start.getY();
}

float line::getY2() {
    return this->end.getY();
}

void line::setX1(float x1) {
    this->start.setX(x1);
}

void line::setX2(float x2) {
    this->end.setX(x2);
}

void line::setY1(float y1) {
    this->start.setY(y1);
}

void line::setY2(float y2) {
    this->end.setY(y2);
}

GraphicsPath* line::createGraphicsPath()
{
    GraphicsPath* path = new GraphicsPath();
    path->AddLine(PointF(start.getX(), start.getY()), PointF(end.getX(), end.getY()));

    return path;
}

void line::getBoundingBox(float& x, float& y, float& width, float& height)
{
    float minX = min(start.getX(), end.getX());
    float minY = min(start.getY(), end.getY());
    float maxX = max(start.getX(), end.getX());
    float maxY = max(start.getY(), end.getY());

    x = minX;
    y = minY;
    width = maxX - minX;
    height = maxY - minY;
}

