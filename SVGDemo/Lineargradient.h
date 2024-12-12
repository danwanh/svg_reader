#pragma once
#ifndef LINEARGRADIENT_H
#define LINEARGRADIENT_H

#include "Lib.h"
#include "Gradient.h"
using namespace std;

class linearGradient : public gradient {
private:
	double x1, y1, x2, y2;
public:
	double getX1() const;
	double getY1() const;
	double getX2() const;
	double getY2() const;

	void setX1(double value);
	void setY1(double value);
	void setX2(double value);
	void setY2(double value);
};
#endif // !LINEARGRADIENT_H
