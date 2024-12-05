#ifndef _STROKE_H_
#define _STROKE_H_
#include "Lib.h"
class stroke {
private:
	MyColor strokeColor;
	float strokeWidth;
public:
	stroke();
	MyColor getStrokeColor();
	float getStrokeWidth();
	void setStrokeColor(MyColor strokeColor);
	void setStrokeWidth(float strokeWidth);

};
#endif // !_STROKE_H_
