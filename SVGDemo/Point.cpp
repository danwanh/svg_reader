#include "stdafx.h"
#include "Point.h"

point::point() {
	x = 0;
	y = 0;
}

point::point(float xCoord, float yCoord) : x(xCoord), y(yCoord) {}

float point::getX() const{
	return this->x;
}

float point::getY() const{
	return this->y;
}

void point::setX(float x) {
	this->x = x;
}

void point::setY(float y) {
	this->y = y;
}