#include "stdafx.h"
#include "Path.h"
path::path() : Shape(), strokeMiterLimit(4.0), fillRule("nonzero"){
	name = "path";
}
vector<pair<string, vector<point>>> path::getPath() {
	return this->pathVct;
}
void path::setPath(vector<pair<string, vector<point>>> P) {
	this->pathVct = P;
}

double path::getStrokeMiterLimit() const
{
	return strokeMiterLimit;
}

void path::setStrokeMiterLimit(double limit)
{
	this->strokeMiterLimit = limit;
}

string path::getFillRule() const
{
	return fillRule;
}

void path::setFillRude(const string& fillrude)
{
	this->fillRule = fillrude;
}

GraphicsPath* path::createGraphicsPath()
{
	return nullptr;
}

void path::getBoundingBox(float& x, float& y, float& width, float& height)
{
}



