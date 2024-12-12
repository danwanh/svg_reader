#include "stdafx.h"
#include "Figure.h"

vector<Shape*> Figure::getList() {
	return list;
}
void Figure::setList(vector<Shape*> list) {
	this->list = list;
}

Figure::~Figure() {
	for (Shape* shape : list) {
		if(shape){
			delete shape;
		}
	}
}