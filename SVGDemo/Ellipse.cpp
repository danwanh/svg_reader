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

GraphicsPath* ellipse::createGraphicsPath()
{
    GraphicsPath* path = new GraphicsPath();

    // Thêm hình ellipse vào GraphicsPath
    float left = c.getX() - rx; // Tọa độ x của góc trái trên
    float top = c.getY() - ry; // Tọa độ y của góc trái trên
    float width = 2 * rx;      // Chiều rộng
    float height = 2 * ry;     // Chiều cao

    path->AddEllipse(left, top, width, height);
    return path;
}

void ellipse::getBoundingBox(float& x, float& y, float& width, float& height) {
    // Tính toán tọa độ góc trên trái
    x = c.getX() - rx; // Tọa độ x của góc trái trên
    y = c.getY() - ry; // Tọa độ y của góc trái trên

    // Tính chiều dài và chiều rộng của bounding box
    width = 2 * rx;    // Chiều rộng là đường kính ngang
    height = 2 * ry;   // Chiều cao là đường kính dọc
}


