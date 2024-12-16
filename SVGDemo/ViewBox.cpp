#include "stdafx.h"
#include "ViewBox.h"

ViewBox::ViewBox() : minX(0), minY(0), width(0), height(0) {}

ViewBox::ViewBox(float x, float y, float w, float h)
{
	minX = x;
	minY = y;
	width = w;
	height = h;
}

float ViewBox::getMinX() const
{
	return minX;
}

float ViewBox::getMinY() const
{
	return minY;
}

float ViewBox::getWidth() const
{
	return width;
}

float ViewBox::getHeight() const
{
	return height;
}

void ViewBox::setMinX(float x)
{
	minX = x;
}

void ViewBox::setMinY(float y)
{
	minY = y;
}

void ViewBox::setWidth(float w)
{
	width = w;
}

void ViewBox::setHeight(float h)
{
	height = h;
}

