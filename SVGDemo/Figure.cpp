#include "stdafx.h"
#include "Figure.h"

vector<Shape*> Figure::getList() {
	return list;
}
void Figure::setList(vector<Shape*> list) {
	this->list = list;
}

void Figure::calCenter(float& xCenter, float& yCenter) {
	float xMax = -FLT_MAX, xMin = FLT_MAX, yMax = -FLT_MAX, yMin = FLT_MAX;
	for (Shape* shape : list) {
		shape->calculateBoundingBox();
	}
	for (Shape* shape : list) {
		if (shape->getXmin() < xMin) {
			xMin = shape->getXmin();
		}
		if (shape->getYmin() < yMin) {
			yMin = shape->getYmin();
		}
		if (shape->getXmax() > xMax) {
			xMax = shape->getXmax();
		}
		if (shape->getYmax() > yMax) {
			yMax = shape->getYmax();
		}
	}
	xCenter = xMin + (xMax - xMin) / 2;
	yCenter = yMin + (yMax - yMin) / 2;
}
Figure::~Figure() {
	for (Shape* shape : list) {
		if(shape){
			delete shape;
		}
	}
}