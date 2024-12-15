#include "stdafx.h"
#include "Group.h"
group::group() : Shape(){
	name = "g";
	parent = NULL;
}
group::group(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> t) : Shape(stroke, strokeW, fill, "g", t){
	parent = NULL;
}
Shape* group::getParent() {
	return parent;
}
void group::setParent(Shape* p) {
	parent = p;
}
vector<Shape*>& group::getChildren() {
	return children;
}
void group::setChildren(vector<Shape*> list) {
	this->children = list;
}
void group::addChild(Shape* shape) {
	children.push_back(shape);
}
group::~group() {
	for (Shape* child : children) {
		delete child;
	}
}

GraphicsPath* group::createGraphicsPath()
{
	return nullptr;
}

void group::getBoundingBox(float& x, float& y, float& width, float& height)
{
}
