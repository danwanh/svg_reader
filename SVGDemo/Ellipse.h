#ifndef _ELLIPSE_H_
#define _ELLIPSE_H_

#include "Shape.h"

using namespace std;

class ellipse : public Shape {
protected:
    point c;
    float rx, ry;

public:
    ellipse();
    ellipse(float centerX, float centerY, float radiusX, float radiusY, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t);

    float getCx();
    float getCy();
    float getRx();
    float getRy();

    void setCx(float cx);
    void setCy(float cy);
    void setRx(float rx);
    void setRy(float ry);

    void calculateBoundingBox() override;
};
#endif