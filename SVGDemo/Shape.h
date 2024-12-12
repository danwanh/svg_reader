#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Lib.h"
#include "TransformCommand.h"
#include "Stroke.h"
using namespace std;

class Shape {
protected:
    stroke str;
    MyColor fillColor;
    string name;
    vector<TransformCommand> transform; //Transformation
public:
    Shape();
    Shape(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> transform);


    MyColor& getFillColor();
    stroke& getStroke();
    string getName();
    void setFillColor(MyColor fillColor);
    void setName(string name);
    void setStroke(stroke& s);

    int nameTonum();
    vector<TransformCommand> getTransform() const;
    void setTransform(vector<TransformCommand>& other);
    void applyTransform(Graphics& graphics);
   

    virtual ~Shape() {

    }
};
#endif