#ifndef _TRANSFORMCOMMAND_H_
#define _TRANSFORMCOMMAND_H_
#include "Lib.h"
class TransformCommand {
private:
	string name;
	float transX, transY;
	float angle;    
	float scaleX, scaleY;
public:
	TransformCommand();
	TransformCommand(string n, float x, float y, float angle, float sx, float sy);

	void setName(string s);
	void setTranslate(float x, float y);
	void setScale(float sx, float sy);
	void setScale(float d);

	void setRotate(float a);

	string getName();
	float getTransX();
	float getTransY();
	float getAngle();
	float getScaleX();
	float getScaleY();

};
#endif // !_TRANSFORMCOMMAND_H_

