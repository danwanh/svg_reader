#include "stdafx.h"
#include "TransformCommand.h"
TransformCommand::TransformCommand() : name(""), transX(0), transY(0), angle(0), scaleX(1), scaleY(1) {

}
TransformCommand::TransformCommand(string n, float x, float y, float a, float sx, float sy) : name(n), transX(x), transY(y), angle(a), scaleX(sx), scaleY(sy) {

}
void TransformCommand::setName(string s) {
	this->name = s;
}
void TransformCommand::setTranslate(float x, float y) {
	name = "translate";
	transX = x;
	transY = y;
	angle = 0;
	scaleX = 1;
	scaleY = 1;
}
void TransformCommand::setScale(float sx, float sy) {
	name = "scale";
	transX = 0;
	transY = 0;
	angle = 0;
	scaleX = sx;
	scaleY = sy;
}
void TransformCommand::setScale(float d) {
	name = "scale";
	transX = 0;
	transY = 0;
	angle = 0;
	scaleX = d;
	scaleY = d;
}
void TransformCommand::setRotate(float a) {
	name = "rotate";
	transX = 0;
	transY = 0;
	angle = a;
	scaleX = 1;
	scaleY = 1;
}
string TransformCommand::getName() {
	return name;
}
float TransformCommand::getTransX() {
	return transX;

}
float TransformCommand::getTransY() {
	return transY;
}
float TransformCommand::getAngle() {
	return angle;
}
float TransformCommand::getScaleX() {
	return scaleX;
}
float TransformCommand::getScaleY() {
	return scaleY;
}
//void TransformCommand::getCommand() {
//	return (*this);
//}