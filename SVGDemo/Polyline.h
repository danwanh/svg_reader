#ifndef _POLYLINE_H_
#define _POLYLINE_H_

#include "Shape.h"
class polyline : public Shape {
private:
    vector<point> points;

public:
    polyline();
    polyline(std::vector<point> pts, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t);

    vector<point> getPoints();

    void setPoints(vector<point> points);    

    GraphicsPath* createGraphicsPath() override;
    void getBoundingBox(float& x, float& y, float& width, float& height) override;
};
#endif 