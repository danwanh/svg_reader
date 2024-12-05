#ifndef _CIRCLE_H_
#define _CIRCLE_H_
#include "Shape.h"
#include "Ellipse.h"
using namespace std;

class circle : public ellipse {

public:
    circle();
    circle(float centerX, float centerY, float r, MyColor stroke, float strokeW, MyColor fill, string name, vector<TransformCommand> t);

    float getRadius();
    void setRadius(float radius);
};

#endif