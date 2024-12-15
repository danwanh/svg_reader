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

//coi lại
GraphicsPath* polygon::createGraphicsPath()
{
    GraphicsPath* path = new GraphicsPath();

    if (!points.empty()) {
        vector<PointF> gdiPoints; // Tọa độ GDI+
        for (const auto& pt : points) {
            gdiPoints.emplace_back(pt.getX(), pt.getY());
        }

        // Tạo một đa giác trong GraphicsPath
        path->AddPolygon(gdiPoints.data(), static_cast<INT>(gdiPoints.size()));
    }

    return path;
}

//coi lại
void polygon::getBoundingBox(float& x, float& y, float& width, float& height)
{
    if (points.empty()) {
        x = y = width = height = 0;
        return;
    }

    // Tìm giá trị nhỏ nhất và lớn nhất của tọa độ x và y
    float minX = points[0].getX();
    float maxX = points[0].getX();
    float minY = points[0].getY();
    float maxY = points[0].getY();

    for (const auto& pt : points) {
        if (pt.getX() < minX) minX = pt.getX();
        if (pt.getX() > maxX) maxX = pt.getX();
        if (pt.getY() < minY) minY = pt.getY();
        if (pt.getY() > maxY) maxY = pt.getY();
    }

    x = minX;
    y = minY;
    width = maxX - minX;
    height = maxY - minY;
}
