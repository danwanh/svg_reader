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
    float xmin, ymin, xmax, ymax;  // Bounding box attributes
    string name;
    vector<TransformCommand> transform; //Transformation
public:
    Shape();
    Shape(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> transform);

    MyColor getStrokeColor();
    float getStrokeWidth();
    MyColor getFillColor();
    float getXmin();
    float getXmax();
    float getYmin();
    float getYmax();
    string getName();

    void setStrokeColor(MyColor strokeColor);
    void setStrokeWidth(float strokeWidth);
    void setFillColor(MyColor fillColor);
    void setXmin(float x);
    void setXmax(float x);
    void setYmin(float y);
    void setYmax(float y);
    void setName(string name);

    
    float getTranslateX() const;
    float getTranslateY() const;
    float getRotationAngle() const;
    float getScaleX() const;
    float getScaleY() const;

    void setTranslate(float tx, float ty);
    void setRotate(float angle);
    void setScale(float sx, float sy);
    void setScaleD(float d);

    int nameTonum();
    virtual void calculateBoundingBox() = 0;
    
    vector<TransformCommand> getTransform() const;
    void setTransform(vector<TransformCommand> &other);
    void applyTransform(Graphics &graphics);
    
    void addTranslate(float x, float y);
    void addRotate(float a);
    void addScale(float x, float y);
    void addScale(float d);

    virtual ~Shape() {

    }
};
#endif