#ifndef _FIGURE_H_
#define _FIGURE_H_
#include "Shape.h"
#include "Gradient.h"
#include "LinearGradient.h"
#include "RadialGradient.h"
class Figure {
private:
	vector<Shape*> list;
	map<string, gradient*> grad;
public:
	vector<Shape*> getList();
	void setList(vector<Shape*> list);
	~Figure();
};
#endif

