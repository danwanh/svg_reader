#include "stdafx.h"
#include "Polygon.h"

polygon::polygon() {
    str.setStrokeWidth(0);
};

polygon::polygon(std::vector<point> pts, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t)  : Shape(stroke, strokeW, fill, name, t), points(pts) {
}

vector<point> polygon::getPoints() {
    return this->points;
}

void polygon::setPoints(vector<point> points) {
    this->points = points;
}
