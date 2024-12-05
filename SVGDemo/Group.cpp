#include "stdafx.h"
#include "Group.h"
group::group() : Shape(){
	name = "g";
}
group::group(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> t) : Shape(stroke, strokeW, fill, "g", t){

}
vector<Shape*>& group::getChildren() {
	return children;
}
void group::setChildren(vector<Shape*> list) {
	this->children = list;
}

void group::calculateBoundingBox() {
	float xMax = -FLT_MAX, xMin = FLT_MAX, yMax = -FLT_MAX, yMin = FLT_MAX;
	for (Shape* shape : children) {
		shape->calculateBoundingBox();
	}
	for (Shape* shape : children) {
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
	this->xmin = xMin;
	this->xmax = xMax;
	this->ymin = yMin;
	this->ymax = yMax;
}
void group::addChild(Shape* shape) {
	children.push_back(shape);
}
group::~group() {
	for (Shape* child : children) {
		delete child;
	}
}