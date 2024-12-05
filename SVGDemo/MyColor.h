#pragma once
#ifndef _MYCOLOR_H_
#define _MYCOLOR_H_

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

class MyColor {
private:
	int r, g, b;
	float opacity;

public:
	MyColor();
	MyColor(int red, int green, int blue, float op);

	int getRed();
	int getGreen();
	int getBlue();
	float getOpacity();

	void setRed(int red);
	void setGreen(int green);
	void setBlue(int blue);
	void setOpacity(float opacity);
};


#endif