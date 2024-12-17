#include "stdafx.h"
#include "ViewBox.h"

ViewBox::ViewBox() : portHeight(0), portWidth(0), minX(0), minY(0), width(100), height(100) {}

float ViewBox::getMinX() const
{
	return minX;
}

float ViewBox::getMinY() const
{
	return minY;
}
float ViewBox::getPortHeight() const
{
	return portHeight;
}

float ViewBox::getPortWidth() const
{
	return portWidth;
}
float ViewBox::getViewWidth() const
{
	return width;
}

float ViewBox::getViewHeight() const
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
void ViewBox::setPortHeight(float x)
{
	portHeight = x;
}

void ViewBox::setPortWidth(float y)
{
	portWidth = y;
}
void ViewBox::setViewWidth(float w)
{
	width = w;
}

void ViewBox::setViewHeight(float h)
{
	height = h;
}

