#include "stdafx.h"
#include "RadialGradient.h"

double radialGradient::getCx() const
{
    return cx;
}

void radialGradient::setCx(double value)
{
    cx = value;
}

double radialGradient::getCy() const
{
    return cy;
}

void radialGradient::setCy(double value)
{
    cy = value;
}

double radialGradient::getR() const
{
    return r;
}

void radialGradient::setR(double value)
{
    r = value;
}

double radialGradient::getFx() const
{
    return fx;
}

void radialGradient::setFx(double value)
{
    fx = value;
}

double radialGradient::getFy() const
{
    return fy;
}

void radialGradient::setFy(double value)
{
    fy = value;
}
