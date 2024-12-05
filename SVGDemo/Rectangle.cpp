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
	calculateBoundingBox();
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

void rectangle::calculateBoundingBox(){
	xmin = getRecX();
	ymin = getRecY();
	xmax = getRecX() + width;
	ymax = getRecY() + height;
}
