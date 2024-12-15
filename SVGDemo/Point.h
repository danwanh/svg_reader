#pragma once
#ifndef _POINT_H_
#define _POINT_H_

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class point {
private:
	float x, y;

public:
	point();
	point(float xCoord, float yCoord);

	float getX() const;
	float getY() const;

	void setX(float x);
	void setY(float y);
};

#endif