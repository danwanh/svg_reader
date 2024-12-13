#ifndef RADIALGRADIENT_H
#define RADIALGRADIENT_H

#include "Lib.h"
#include "Gradient.h"
using namespace std;

class radialGradient : public gradient {
private:
    double cx, cy, r, fx, fy;

public:
    double getCx() const;
    void setCx(double value);

    double getCy() const;
    void setCy(double value);

    double getR() const;
    void setR(double value);

    double getFx() const;
    void setFx(double value);

    double getFy() const;
    void setFy(double value);
};
#endif // !RADIALGRADIENT_H
