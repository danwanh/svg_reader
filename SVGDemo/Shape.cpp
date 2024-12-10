#include "stdafx.h"
#include "Shape.h"

Shape::Shape() {
	str.setStrokeWidth(0);
	xmax = 0;
	ymax = 0;
	ymin = 0;
	xmin = 0;
	transform = {};
}

Shape::Shape(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> t) : fillColor(fill), name(shapeName), xmin(0), ymin(0), xmax(0), ymax(0), transform(t) {
	str.setStrokeColor(stroke);
	str.setStrokeWidth(strokeW);
}

MyColor Shape::getStrokeColor() {
	return str.getStrokeColor();
}

float Shape::getStrokeWidth() {
	return str.getStrokeWidth();
}

MyColor& Shape::getFillColor() {
	return this->fillColor;
}

float Shape::getXmax() {
	return this->xmax;
}

float Shape::getXmin() {
	return this->xmin;
}

float Shape::getYmax() {
	return this->ymax;
}

float Shape::getYmin() {
	return this->ymin;
}

string Shape::getName(){
	return this->name;
}

void Shape::setStrokeColor(MyColor strokeColor) {
	str.setStrokeColor(strokeColor);
}

void Shape::setStrokeWidth(float strokeWidth) {
	str.setStrokeWidth(strokeWidth);
}

void Shape::setFillColor(MyColor fillColor) {
	this->fillColor = fillColor;
}

void Shape::setXmin(float x) {
	this->xmin = x;
}

void Shape::setXmax(float x) {
	this->xmax = x;
}

void Shape::setYmin(float y) {
	this->ymin = y;
}

void Shape::setYmax(float y) {
	this->ymax = y;
}

void Shape::setName(string name) {
	this->name = name;
}
int Shape::nameTonum() {
	if (name == "rect") return 1;
	if (name == "circle") return 2;
	if (name == "polygon") return 3;
	if (name == "polyline") return 4;
	if (name == "line") return 5;
	if (name == "text") return 6;
	if (name == "ellipse") return 7;
	if (name == "g") return 8;
	if (name == "path") return 9;
	return -1;
}

vector<TransformCommand> Shape::getTransform() const {
	return transform;
}
void Shape::setTransform(vector<TransformCommand> &other) {
	transform = other;
}
void Shape::applyTransform(Graphics& graphics) {
	for (auto& command : transform) {
		if (command.getName() == "translate") {
			graphics.TranslateTransform(command.getTransX(), command.getTransY());
		}
		else if (command.getName() == "rotate") {
			graphics.RotateTransform(command.getAngle());

		}
		else if (command.getName() == "scale") {
			graphics.ScaleTransform(command.getScaleX(), command.getScaleY());
		}
	}
}
void Shape::addTranslate(float x, float y) {
	TransformCommand trans("translate", x, y, 0, 1, 1);
	transform.push_back(trans);
}
void Shape::addRotate(float a) {
	float xCenter = xmin + (xmax - xmin) / 2;
	float yCenter = ymin + (ymax - ymin) / 2;

	//transform.push_back(TransformCommand("transform", xCenter, yCenter, 0, 1, 1));
	TransformCommand rotate("rotate", 0, 0, a, 1, 1);
	transform.push_back(rotate);	
	//transform.push_back(TransformCommand("transform", -xCenter, -yCenter, 0, 1, 1));

}
void Shape::addScale(float x, float y) {
	TransformCommand sc("scale", 0, 0, 0, x, y);
	transform.push_back(sc);
}
void Shape::addScale(float d) {
	TransformCommand sc("scale", 0, 0, 0, d, d);
	transform.push_back(sc);
}
