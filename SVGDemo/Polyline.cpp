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

GraphicsPath* polyline::createGraphicsPath()
{
    GraphicsPath* path = new GraphicsPath();

    if (points.size() < 2) {
        // Polyline không có đủ điểm để tạo path
        return path;
    }

    // Tạo mảng các điểm (PointF) từ vector<point>
    vector<PointF> gdiplusPoints;
    for (const auto& pt : points) {
        gdiplusPoints.emplace_back(PointF(pt.getX(), pt.getY()));
    }

    // Thêm các đoạn thẳng vào GraphicsPath
    path->AddLines(gdiplusPoints.data(), gdiplusPoints.size());

    return path;
}

void polyline::getBoundingBox(float& x, float& y, float& width, float& height)
{
    if (points.empty()) {
        // Nếu không có điểm nào, bounding box bằng 0
        x = y = width = height = 0;
        return;
    }

    // Tìm giá trị min và max của x, y trong vector<point>
    float minX = points[0].getX(), maxX = points[0].getX();
    float minY = points[0].getY(), maxY = points[0].getY();

    for (const auto& pt : points) {
        minX = min(minX, pt.getX());
        maxX = max(maxX, pt.getX());
        minY = min(minY, pt.getY());
        maxY = max(maxY, pt.getY());
    }

    // Tính toán bounding box
    x = minX;
    y = minY;
    width = maxX - minX;
    height = maxY - minY;
}


