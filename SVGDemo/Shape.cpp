﻿#include"stdafx.h"
#include "Shape.h"

Shape::Shape() {
	str.setStrokeWidth(0);
	transform = {};
}

Shape::Shape(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> t) : fillColor(fill), name(shapeName), transform(t) {
	str.setStrokeColor(stroke);
	str.setStrokeWidth(strokeW);
}


MyColor& Shape::getFillColor() {
	return this->fillColor;
}


string Shape::getName() {
	return this->name;
}

stroke& Shape::getStroke(){
	return str;
}
void Shape::setStroke(stroke &str) {
	this->str = str;
}
void Shape::setGradient(gradient* grad) {
	this->grad = grad;
}

gradient* Shape::getGradient() {
	return grad;
}
void Shape::setFillColor(MyColor fillColor) {
	this->fillColor = fillColor;
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
void Shape::setTransform(vector<TransformCommand>& other) {
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
