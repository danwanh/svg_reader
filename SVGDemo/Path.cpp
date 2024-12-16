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
	float xMax = -FLT_MAX, xMin = FLT_MAX, yMax = -FLT_MAX, yMin = FLT_MAX;
	int size = pathVct.size();
	vector <point> points;
	for (int i = 0; i < size; i++) {
		vector<point> temp = pathVct[i].second;
		int sizeTemp = temp.size();
		for (int j = 0; j < sizeTemp; j++) {
			points.push_back(temp[j]);
		}
	}

	for (point& p : points) {
		if (p.getX() > xMax) {
			xMax = p.getX();
		}
		if (p.getX() < xMin) {
			xMin = p.getX();
		}
		if (p.getY() > yMax) {
			yMax = p.getY();
		}
		if (p.getY() < yMin) {
			yMin = p.getY();
		}
	}
	width = xMax - xMin;
	x = xMin;
	height = yMax - yMin;
	y = yMin;
}



