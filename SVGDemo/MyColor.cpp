#include "stdafx.h"
#include "MyColor.h"

MyColor::MyColor(): r(0), g(0), b(0), opacity(1) {}

MyColor::MyColor(int red, int green, int blue, float op) : r(red), g(green), b(blue), opacity(op) {}

int MyColor::getRed() {
	return this->r;
}

int MyColor::getGreen() {
	return this->g;
}

int MyColor::getBlue() {
	return this->b;
}

float MyColor::getOpacity() {
	return this->opacity;
}

void MyColor::setRed(int red) {
	this->r = red;
}

void MyColor::setGreen(int green) {
	this->g = green;
}

void MyColor::setBlue(int blue) {
	this->b = blue;
}

void MyColor::setOpacity(float opacity) {
	this->opacity = opacity;
}