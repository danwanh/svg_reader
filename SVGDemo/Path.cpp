#include "stdafx.h"
#include "Path.h"
path::path() : Shape() {
	name = "path";
}
vector<pair<string, vector<point>>> path::getPath() {
	return this->pathVct;
}
void path::setPath(vector<pair<string, vector<point>>> P) {
	this->pathVct = P;
}

GraphicsPath* path::createGraphicsPath()
{
	return nullptr;
}

void path::getBoundingBox(float& x, float& y, float& width, float& height)
{
}



