#ifndef _STROKE_H_
#define _STROKE_H_
#include "Lib.h"
class stroke {
private:
	MyColor strokeColor;
	float strokeWidth;
	float strokemiterlimit;
public:
	stroke();
	MyColor& getStrokeColor();
	float getStrokeWidth();
	float getStrokeMiterLimit();
	void setStrokeColor(MyColor strokeColor);
	void setStrokeWidth(float strokeWidth);
	void setStrokeMiterLimit(float strokemiterlimit);

};
#endif // !_STROKE_H_
