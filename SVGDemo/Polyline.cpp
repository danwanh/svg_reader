#include "stdafx.h"
#include "Polyline.h"
polyline::polyline(): Shape() {
    str.setStrokeWidth(0);
}

polyline::polyline(std::vector<point> pts, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t) : Shape(stroke, strokeW, fill, name, t), points(pts) {
}

vector<point> polyline::getPoints() {
    return this->points;
}

void polyline::setPoints(vector<point> points) {
    this->points = points;
}


