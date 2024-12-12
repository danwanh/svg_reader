#ifndef _GRADIENT_H_
#define _GRADIENT_H_
#include "TransformCommand.h"
#include "MyColor.h"

class stop {
public:
    MyColor stopColor; // Màu của điểm dừng (r, g, b, opa)
    double offset;         // Vị trí điểm dừng 
};

class gradient {
private:
    vector<stop> colorStop;
    string id;
    vector<TransformCommand> transform;
    string gradientUnits;
public:
    vector<stop> getColorStop() const;
    void setColorStop(vector<stop>& stops);

    string getId() const;
    void setId(string& newId);

    vector<TransformCommand> getTransform();
    void setTransform(vector<TransformCommand>& commands);

    string getGradientUnits() const;
    void setGradientUnits(string& units);
};
#endif // !_GRADIENT_H_
