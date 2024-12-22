#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Lib.h"
#include "Gradient.h"
#include "TransformCommand.h"
#include "Stroke.h"
using namespace std;

class Shape {
protected:
    stroke str;
    MyColor fillColor;
    string name;
    vector<TransformCommand> transform; 
    gradient* fillGrad;
    gradient* strokeGrad;
    bool hasGradient;
public:
    Shape();
    Shape(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> transform);


    MyColor& getFillColor();
    stroke& getStroke();
    string getName();
    void setFillColor(MyColor fillColor);
    void setName(string name);
    void setStroke(stroke& s);
    void setFillGradient(gradient* grad);
    void setStrokeGradient(gradient* grad);
    void setUsingGradient(bool use);

    gradient* getStrokeGradient();
    gradient* getFillGradient();

    bool isUsingGradient();
    vector<TransformCommand> getTransform() const;
    void setTransform(vector<TransformCommand>& other);
    void applyTransform(Graphics& graphics);
   
    int nameTonum();

    virtual GraphicsPath* createGraphicsPath() = 0; //createGraphicsPath() : để vẽ, tô màu, tính toán.
    virtual void getBoundingBox(float& x, float& y, float& width, float& height) = 0; // //getBoundingBox() : Tính hcn bao quanh để kiểm tra va chạm,tối ưu hóa,căn chỉnh.

    virtual ~Shape(){}
};
#endif