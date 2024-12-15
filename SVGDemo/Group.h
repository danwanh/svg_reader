#ifndef _GROUP_H_
#define _GROUP_H_
#include <vector>
#include "Shape.h"
using namespace std;
class group : public Shape {
private:
	vector<Shape*> children;
	Shape* parent;
public:
	group();
	group(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> t);

	Shape* getParent();
	void setParent(Shape* p);
	vector<Shape*>& getChildren();
	void setChildren(vector<Shape*> list);
	void addChild(Shape* shape);
	~group();

	GraphicsPath* createGraphicsPath() override;
	void getBoundingBox(float& x, float& y, float& width, float& height) override;
};
#endif // !_GROUP_H_

