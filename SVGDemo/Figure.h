#ifndef _FIGURE_H_
#define _FIGURE_H_
#include "Shape.h"
#include <vector>
class Figure {
private:
	vector<Shape*> list;
public:
	vector<Shape*> getList();
	void setList(vector<Shape*> list);

	void calCenter(float&, float&);
};
#endif

