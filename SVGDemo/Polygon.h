
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "Shape.h"

class polygon : public Shape {
private:
    vector<point> points;

public:
    polygon();
    polygon(std::vector<point> pts, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t);

    vector<point> getPoints();

    void setPoints(vector<point> points);

    void calculateBoundingBox() override;
};

#endif