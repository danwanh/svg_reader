#include "stdafx.h"
#include "Stroke.h"
stroke::stroke() {
	strokeWidth = 0;
}
void stroke::setStrokeColor(MyColor strokeColor) {
	this->strokeColor = strokeColor;
}

void stroke::setStrokeWidth(float strokeWidth) {
	this->strokeWidth = strokeWidth;
}
MyColor& stroke::getStrokeColor() {
	return this->strokeColor;
}

float stroke::getStrokeWidth() {
	return this->strokeWidth;
}
