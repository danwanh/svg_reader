#ifndef _PATH_H_
#define _PATH_H_

#include "Lib.h"
#include "Shape.h"
#include "Point.h"
using namespace std;

class path : public Shape {
private:
	vector<pair<string, vector<point>>> pathVct;
	double strokeMiterLimit;
	string fillRule;
public:
	path();
	vector<pair<string, vector<point>>> getPath();
	void setPath(vector<pair<string, vector<point>>>);

	double getStrokeMiterLimit() const;
	void setStrokeMiterLimit(double limit);

	string getFillRule() const;
	void setFillRude(const string& fillrude);

	GraphicsPath* createGraphicsPath() override;
	void getBoundingBox(float& x, float& y, float& width, float& height) override;
};

#endif // !PATH_H
