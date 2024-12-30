#include "stdafx.h"
#include "Stroke.h"
stroke::stroke() {
	strokeWidth = 0;
	strokemiterlimit = 4;
}
void stroke::setStrokeColor(MyColor strokeColor) {
	this->strokeColor = strokeColor;
}

void stroke::setStrokeWidth(float strokeWidth) {
	this->strokeWidth = strokeWidth;
}

void stroke::setStrokeMiterLimit(float strokemiterlimit) {
	this->strokemiterlimit = strokemiterlimit;
}
MyColor& stroke::getStrokeColor() {
	return this->strokeColor;
}

float stroke::getStrokeWidth() {
	return this->strokeWidth;
}
float stroke::getStrokeMiterLimit() {
	return this->strokemiterlimit;
}