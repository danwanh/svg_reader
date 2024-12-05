#include "stdafx.h"
#include "Polygon.h"

polygon::polygon() {
    str.setStrokeWidth(0);
};

polygon::polygon(std::vector<point> pts, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t)  : Shape(stroke, strokeW, fill, name, t), points(pts) {
    calculateBoundingBox();
}

vector<point> polygon::getPoints() {
    return this->points;
}

void polygon::setPoints(vector<point> points) {
    this->points = points;
}

void polygon::calculateBoundingBox() {
    if (points.empty()) return;
    xmin = points[0].getX();
    xmax = points[0].getX();
    ymin = points[0].getY();
    ymax = points[0].getY();
    for (int i = 0; i < points.size(); i++) {
        xmin = min(xmin, points[i].getX());
        xmax = max(xmax, points[i].getX());
        ymin = min(ymin, points[i].getY());
        ymax = max(ymax, points[i].getY());
    }
}
