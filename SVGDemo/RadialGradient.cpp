#include "stdafx.h"
#include "RadialGradient.h"

radialGradient::radialGradient()
{
    cx = 0.5;
    cy = 0.5;
    r = 0.5;
    fx = 0.5;
    fy = 0.5;
    fr = 0;
    type = GradientType::RADIAL;
}

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

double radialGradient::getFr() const
{
    return fr;
}

void radialGradient::setFr(double value)
{
    fr = value;
}
