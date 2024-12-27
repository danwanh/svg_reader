#ifndef _FIGURE_H_
#define _FIGURE_H_
#include "Shape.h"
#include "Gradient.h"
#include "LinearGradient.h"
#include "RadialGradient.h"
#include "ViewBox.h"
class Figure {
private:
	ViewBox* vb;
	vector<Shape*> list;
	map<string, gradient*> grad;
public:
	void setViewBox(ViewBox* vb_);
	ViewBox* getViewBox();
	vector<Shape*> getList();
	void setList(vector<Shape*> list);
	~Figure();
};
#endif

