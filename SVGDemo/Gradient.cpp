#include "stdafx.h"
#include "Gradient.h"
gradient::gradient() {
    id = "";
    gradientUnits = "userOnSpaceUse";
    spreadMethod = "";
}
vector<stop> gradient::getColorStop() const {
    return colorStop;
}

void gradient::setColorStop(vector<stop>& stops) {
    colorStop = stops;
}

string gradient::getId() const {
    return id;
}

void gradient::setId(string& newId) {
    id = newId;
}

vector<TransformCommand> gradient::getTransform() {
    return transform;
}

void gradient::setTransform(vector<TransformCommand>& commands) {
    transform = commands;
}

string gradient::getGradientUnits() const {
    return gradientUnits;
}
void gradient::setGradientUnits(string units) {
    gradientUnits = units;
}
GradientType gradient::getType() const {
    return type;
}
void gradient::setType(GradientType t) {
    type = t;
}
string gradient::getSpreadMethod() const {
    return spreadMethod;
}
void gradient::setSpreadMethod(string method) {
    spreadMethod = method;
}