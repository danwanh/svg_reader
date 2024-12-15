#include "stdafx.h"
#include "Rectangle.h"

rectangle::rectangle() {
	start.setX(0);
	start.setY(0);
	width = 0;
	height = 0;
}

rectangle::rectangle(float x, float y, float w, float h, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t) : Shape(stroke, strokeW, fill, name, t),  width(w), height(h) {
	start.setX(x);
	start.setY(y);
}

float rectangle::getRecX() {
	return this->start.getX();
}

float rectangle::getRecY() {
	return this->start.getY();
}

float rectangle::getWidth() {
	return this->width;
}

float rectangle::getHeight() {
	return this->height;
}

void rectangle::setRecX(float x) {
	start.setX(x);
}

void rectangle::setRecY(float y) {
	start.setY(y);
}

void rectangle::setWidth(float width) {
	this->width = width;
}

void rectangle::setHeight(float height) {
	this->height = height;
}

GraphicsPath* rectangle::createGraphicsPath()
{
	GraphicsPath* path = new GraphicsPath();

	// Xác định tọa độ và kích thước của hình chữ nhật
	float x = start.getX();
	float y = start.getY();

	// Thêm hình chữ nhật vào GraphicsPath
	path->AddRectangle(RectF(x, y, width, height));

	return path;
}

void rectangle::getBoundingBox(float& x, float& y, float& width, float& height)
{
	// Tọa độ góc trái trên và kích thước chính là bounding box
	x = start.getX();
	y = start.getY();
	width = this->width;
	height = this->height;
}

